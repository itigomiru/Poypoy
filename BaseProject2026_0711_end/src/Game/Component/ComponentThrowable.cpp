#pragma once
#include "ComponentThrowable.h"
#include <Game/PoyPoy_Throwable.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentCollisionBox.h>
#include <fstream>
#include <cmath>

void ComponentThrowable::Init()
{
    __super::Init();

    // オーナーが投げ物ベースを持っていたら、その設計値をこのコンポーネントの動作値として受け取る
    if(auto throwable = std::dynamic_pointer_cast<PoyPoy::Throwable>(GetOwnerPtr())) {
        throw_speed_   = throwable->GetThrowSpeed();
        weight_        = throwable->GetWeight();
        bounce_        = throwable->GetBounce();
        friction_      = throwable->GetFriction();
        gravity_accel_ = throwable->GetGravityAccel();
        spin_          = throwable->GetSpin();
    }

    // シーンに設定された値をコリジョンに反映する
    ApplySettings();

    // オーナーのコリジョンに当たり処理を仕掛ける(カプセル・ボックス両対応。Rockはボックス)
    auto collision = owner_->GetComponent<ComponentCollision>();
    if(collision) {
        std::weak_ptr<ComponentThrowable> weak = std::dynamic_pointer_cast<ComponentThrowable>(shared_from_this());
        collision->OnHitFunc                    = [weak](const ComponentCollision::HitInfo& hit_info) {
            if(auto self = weak.lock())
                self->OnContact(hit_info);
        };
    }
}

void ComponentThrowable::Update()
{
    __super::Update();

    // オーナーのコリジョンのグループは常にTHROWABLE(カプセル・スフィア・ボックス対応。Rockはスフィア)
    if(auto capsule = owner_->GetComponent<ComponentCollisionCapsule>())
        capsule->SetCollisionGroup(ComponentCollision::CollisionGroup::THROWABLE);
    else if(auto sphere = owner_->GetComponent<ComponentCollisionSphere>())
        sphere->SetCollisionGroup(ComponentCollision::CollisionGroup::THROWABLE);
    else if(auto box = owner_->GetComponent<ComponentCollisionBox>())
        box->SetCollisionGroup(ComponentCollision::CollisionGroup::THROWABLE);

    // 攻撃判定は当たったフレームのノックバック処理より先に消えないよう、Updateのこの時点(当たり処理の前)で消す
    if(disarm_pending_) {
        armed_          = false;
        disarm_pending_ = false;
    }

    // 持ち上げ中は重力も移動も行わない(位置はComponentPickUpが制御する)
    if(held_) {
        velocity_ = {0.0f, 0.0f, 0.0f};
        grounded_ = false;
        return;
    }

    // 前フレームの当たり結果を使う(今フレームの当たり結果はPrePhysicsで更新される)
    bool was_grounded = grounded_;
    grounded_         = false;

    float dt = update_delta_time_ > 0.0f ? update_delta_time_ : 1.0f / 60.0f;

    // 接地しているときは地面へ向かう速度を取り消す
    if(was_grounded && velocity_.y < 0.0f)
        velocity_.y = 0.0f;

    // 重力を速度に加える
    velocity_ += float3(0.0f, -gravity_accel_, 0.0f) * dt;

    // 接地中は地面摩擦を毎フレームの水平速度に掛ける
    // (接触ハンドラ内の摩擦だけでは接触が途切れたフレームは減速せず、ボックス等はいつまでも滑り続ける)
    if(was_grounded) {
        float damp = 1.0f - friction_ * dt;
        if(damp < 0.0f)
            damp = 0.0f;
        velocity_.x *= damp;
        velocity_.z *= damp;

        // 摩擦で止まりかけた微小な横滑りは0にして確実に静止させる
        if(sqrtf(velocity_.x * velocity_.x + velocity_.z * velocity_.z) < 0.5f) {
            velocity_.x = 0.0f;
            velocity_.z = 0.0f;
        }
    }

    // 速度ぶん移動する
    owner_->SetTranslate(owner_->GetWorldMatrix().translate() + velocity_ * dt);

    // 飛行中は長い軸(ローカルY軸)回りに回転する。向きは進行方向(軸と直交する動き)で決まり、逆向きへ動けば反転する
    if(spin_rate_ != 0.0f) {
        float3 roll_dir = cross(spin_axis_, {0.0f, 1.0f, 0.0f});    // 軸×上方向=このスピンで前に転がる進行方向
        if((float)dot(velocity_, roll_dir) * spin_rate_ < 0.0f)
            spin_rate_ = -spin_rate_;

        if(was_grounded) {
            float spin_damp = 1.0f - friction_ * dt;
            spin_rate_      = spin_damp > 0.0f ? spin_rate_ * spin_damp : 0.0f;
        }
        if(spin_rate_ != 0.0f) {
            spin_angle_ += spin_rate_ * dt;
            if(spin_angle_ > 6.28318531f)
                spin_angle_ -= 6.28318531f;
            float c = cosf(spin_angle_);
            float s = sinf(spin_angle_);
            owner_->SetVectorAxisX(spin_base_x_ * c + cross(spin_axis_, spin_base_x_) * s);
            owner_->SetVectorAxisZ(spin_base_z_ * c + cross(spin_axis_, spin_base_z_) * s);
        }
    }

    // デバッグログ(throwable_debug.csv)に1フレーム分の状態を書き出す
    {
        static std::ofstream dbg;
        static int           dbg_frame = 0;
        if(!dbg.is_open()) {
            dbg.open("F:/SatoKoki/teamProject/PPP/BaseProject2026_0711_end/throwable_debug.csv", std::ios::trunc);
            dbg << "frame,name,held,grounded,px,py,pz,vx,vy,vz,pen,nx,ny,nz,contacts,friction,dt\n";
        }
        if(dbg.is_open()) {
            auto mat = owner_->GetWorldMatrix().translate();
            dbg << dbg_frame++ << ',' << owner_->GetName().data() << ",0," << (was_grounded ? 1 : 0) << ',' << mat.x << ',' << mat.y << ',' << mat.z << ','
                << velocity_.x << ',' << velocity_.y << ',' << velocity_.z << ',' << contact_pen_ << ',' << contact_normal_.x << ',' << contact_normal_.y << ','
                << contact_normal_.z << ',' << dbg_contacts_ << ',' << friction_ << ',' << dt << '\n';
            dbg.flush();
        }
        dbg_contacts_ = 0;
    }
}

void ComponentThrowable::OnContact(const ComponentCollision::HitInfo& hitInfo)
{
    if(held_ || !hitInfo.hit_ || !hitInfo.collision_ || !hitInfo.hit_collision_)
        return;

    dbg_contacts_++;

    // 投げ物のコリジョン(カプセル・スフィア・ボックス。形状を問わず基底型で扱う)
    auto collision = std::dynamic_pointer_cast<ComponentCollision>(hitInfo.collision_);
    if(!collision)
        return;

    // オーバーラップ設定の相手は素通りさせる
    if(collision->IsOverlap(hitInfo.hit_collision_->GetCollisionGroup()) || hitInfo.hit_collision_->IsOverlap(collision->GetCollisionGroup()))
        return;

    // 実ヒットした攻撃判定はこの当たりフレームの終わりで消す
    disarm_pending_ = true;

    // オーバーラップでない実ヒットなので、爆発などのトリガーを呼ぶ
    if(contact_func_)
        contact_func_();

    // 渡されたpush_は質量按分されて減るので、100%分のめり込み量を自分で調べる。
    // エンジンが既に位置補正を済ませていると計測値は0になるため、その場合は
    // dispatch時点の当たりベクトル、それも0なら直前の接触法線で接地として扱う
    ComponentCollision::HitInfo probe = collision->IsHit(hitInfo.hit_collision_);

    float3 src{};
    float  len = 0.0f;
    if(probe.hit_) {
        src = probe.push_;
        len = (float)sqrtf(dot(src, src));
    }
    if(len <= 0.0f) {
        src = hitInfo.push_;
        len = (float)sqrtf(dot(src, src));
    }
    if(len <= 0.0f) {
        src = contact_normal_;
        len = (float)sqrtf(dot(src, src));
        if(len <= 0.0f)
            return;
    }

    float3 normal = src * (1.0f / len);

    contact_normal_ = normal;
    contact_pen_    = len;

    // 自分で計測しためり込み分だけ押し戻す。計測0なら位置は解決済みなので押し戻さない。
    // 大きすぎる押し戻しは跳ねて見えるので上限する
    if(probe.hit_ && len > 0.0f) {
        float push_len = len > 2.0f ? 2.0f : len;
        if(push_len > 0.01f)
            owner_->SetTranslate(owner_->GetWorldMatrix().translate() + normal * push_len);
    }

    float dt = update_delta_time_ > 0.0f ? update_delta_time_ : 1.0f / 60.0f;

    float vn = dot(velocity_, normal);
    if(vn < 0.0f) {
        // 1フレーム分の重力が生む程度の接触速度ならバウンドではなく接地として扱う
        float contact_limit = gravity_accel_ * dt * 1.5f;
        if(vn < -contact_limit && bounce_ > 0.0f) {
            velocity_  -= normal * vn * (1.0f + bounce_);    // バウンド
            spin_rate_  = -spin_rate_;                       // 跳ね返るたびに逆回転へ
        }
        else
            velocity_ -= normal * vn;    // 吸収
    }

    // 摩擦は接線方向の速度にだけ働く
    float3 tangent = velocity_ - normal * dot(velocity_, normal);
    float  damp    = 1.0f - friction_ * dt;
    velocity_      = normal * dot(velocity_, normal) + tangent * (damp > 0.0f ? damp : 0.0f);

    // 接地時に極端に遅い速度は停止として扱い、摩擦で確実に止める
    if(normal.y > 0.5f && (float)length(velocity_) < 0.5f)
        velocity_ = {0.0f, 0.0f, 0.0f};

    if(normal.y > 0.5f) {
        grounded_ = true;

        // 押し出し後のコリジョン下面の位置を地面の高さとして記録する(カプセル・ボックスで形状計算が違う)
        float  pos_y   = owner_->GetWorldMatrix().translate().y;
        float  bottom  = 0.0f;    //!< 形状の中心から下面までの距離(マイナス)
        if(auto capsule = std::dynamic_pointer_cast<ComponentCollisionCapsule>(collision)) {
            matrix m    = owner_->GetWorldMatrix();
            float3 axis = normalize((float3&)m.axisVectorY());
            float  drop = axis.y * capsule->GetHeight();
            if(drop > 0.0f)
                drop = 0.0f;
            bottom = drop - capsule->GetRadius();
        }
        else if(auto sphere = std::dynamic_pointer_cast<ComponentCollisionSphere>(collision)) {
            bottom = -sphere->GetRadius() * owner_->GetScaleAxisXYZ().y;
        }
        else if(auto box = std::dynamic_pointer_cast<ComponentCollisionBox>(collision)) {
            // Boxは当たり判定上 GetSize() が半サイズなので下面は -size.y ぶん
            bottom = -box->GetSize().y * owner_->GetScaleAxisXYZ().y;
        }
        ground_height_ = pos_y + bottom;
    }
}

void ComponentThrowable::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();

        if(ImGui::TreeNode("Throwable")) {
            // 有効/無効
            bool enable = GetStatus(StatusBit::Enable);
            if(ImGui::Checkbox(u8"有効", &enable))
                SetStatus(StatusBit::Enable, enable);

            // 挙動の設定
            bool changed  = ImGui::DragFloat(u8"初速度(throw speed)", &throw_speed_, 0.1f, 0.0f, 100.0f, "%.1f");
            changed      |= ImGui::DragFloat(u8"重さ(weight)", &weight_, 0.1f, 0.1f, 1000.0f, "%.1f");
            changed      |= ImGui::DragFloat(u8"バウンドの強さ(restitution)", &bounce_, 0.01f, 0.0f, 1.0f, "%.2f");
            changed      |= ImGui::DragFloat(u8"摩擦の強さ(friction)", &friction_, 0.01f, 0.0f, 100.0f, "%.2f");
            changed      |= ImGui::DragFloat(u8"重力加速度(gravity)", &gravity_accel_, 0.1f, 0.0f, 500.0f, "%.1f");
            changed      |= ImGui::DragFloat(u8"回転の速さ(spin deg/s)", &spin_, 1.0f, 0.0f, 720.0f, "%.0f");
            if(changed)
                ApplySettings();

            ImGui::Text("velocity: %.2f, %.2f, %.2f", velocity_.x, velocity_.y, velocity_.z);
            ImGui::Text("groundY: %.1f  pen: %.2f  n: %.2f,%.2f,%.2f", ground_height_, contact_pen_, contact_normal_.x, contact_normal_.y, contact_normal_.z);

            // GUI上でオーナーから自分を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void ComponentThrowable::PickUp()
{
    held_     = true;
    grounded_ = false;
    velocity_ = {0.0f, 0.0f, 0.0f};

    // 持っている間は攻撃判定なし
    armed_          = false;
    disarm_pending_ = false;
}

void ComponentThrowable::Throw(const float3& dir)
{
    held_     = false;
    grounded_ = false;

    // 投げた瞬間から攻撃判定ON(当たって消えるか拾い直されるまで)
    armed_          = true;
    disarm_pending_ = false;

    // 水平方向へ設定された初速度を与える
    float3 direction = dir;
    direction.y      = 0.0f;
    float len        = sqrtf(dot(direction, direction));
    velocity_        = len > 0.0f ? direction * (throw_speed_ / len) : float3(0.0f, 0.0f, 0.0f);

    // 投げた瞬間の姿态を捕捉し、長い軸(ローカルY軸)回りの回転をそこからの相対で掛ける
    spin_axis_   = owner_->GetVectorAxisY();
    spin_base_x_ = owner_->GetVectorAxisX();
    spin_base_z_ = owner_->GetVectorAxisZ();
    // 進行方向(軸と直交する投げ方向の成分)が正方向なら正回転、逆向きなら逆回転
    float3 roll_dir = cross(spin_axis_, {0.0f, 1.0f, 0.0f});
    spin_rate_      = spin_ * (3.14159265f / 180.0f) * ((float)dot(velocity_, roll_dir) >= 0.0f ? 1.0f : -1.0f);
    spin_angle_     = 0.0f;
}

void ComponentThrowable::ApplySettings()
{
    // 重さをコリジョンの質量に反映(カプセル・スフィア・ボックス対応)
    if(auto capsule = owner_->GetComponent<ComponentCollisionCapsule>())
        capsule->SetMass(weight_);
    else if(auto sphere = owner_->GetComponent<ComponentCollisionSphere>())
        sphere->SetMass(weight_);
    else if(auto box = owner_->GetComponent<ComponentCollisionBox>())
        box->SetMass(weight_);
}

CEREAL_REGISTER_TYPE(ComponentThrowable)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentThrowable)
