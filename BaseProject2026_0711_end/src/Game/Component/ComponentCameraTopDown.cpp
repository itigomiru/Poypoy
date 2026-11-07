#include <Game/Component/ComponentCameraTopDown.h>
#include <System/Component/ComponentCamera.h>
#include <System/ImGui.h>

#include <cmath>

void ComponentCameraTopDown::Init()
{
    __super::Init();

    focus_ = focus_center_;
}

void ComponentCameraTopDown::Update()
{
    __super::Update();

    auto cam = GetOwner()->GetComponent<ComponentCamera>();
    if(!cam)
        return;

    // 名前で指定された追従対象を毎フレーム解決する
    if(!follow_.lock() && !follow_name_.empty()) {
        follow_ = Scene::Object::Get<Object>(follow_name_);
    }

    // 注視点: 固定中心 + キャラクター位置への寄せ(追従用の仕組み)
    float3 want = focus_center_;
    if(auto target = follow_.lock(); target && follow_weight_ > 0.0f) {
        float w = follow_weight_;
        if(w > 1.0f)
            w = 1.0f;
        if(w < 0.0f)
            w = 0.0f;
        float3 tp = target->GetWorldMatrix().translate().xyz;
        want      = focus_center_ * (1.0f - w) + tp * w;
    }

    // 注視点をなめらかに追う
    float f = smooth_ * GetDeltaTime60() / 60.0f;
    if(f > 1.0f)
        f = 1.0f;
    if(f < 0.0f)
        f = 0.0f;
    focus_ = focus_ + (want - focus_) * f;

    // 見下ろし角度(水平からの角度) lookAtLHの世界上方向と平行になる真真下(90度)は手前へ寄って回避する
    float p = pitch_;
    if(p < 20.0f)
        p = 20.0f;
    if(p > 88.0f)
        p = 88.0f;
    float pr = p * DegToRad;
    float yr = screen_yaw_ * DegToRad;

    // 画面上が向く世界方向
    float3 screen_up = {sinf(yr), 0.0f, cosf(yr)};

    // 焦点からカメラへの向き(カメラは画面上の反対側に位置する)
    float3 dir    = {-screen_up.x * cosf(pr), sinf(pr), -screen_up.z * cosf(pr)};
    float  dir_len = (float)sqrtf(dot(dir, dir));
    if(dir_len < 0.001f)
        dir_len = 1.0f;
    dir = dir * (1.0f / dir_len);

    float3 position = focus_ + dir * height_;

    // 焦点へ見下ろす(カメラアップは画面上方向を視線に垂直な平面へ射影したもの)
    float3 up     = screen_up - dir * dot(screen_up, dir);
    float  up_len = (float)sqrtf(dot(up, up));
    if(up_len < 0.001f)
        up = {0.0f, 1.0f, 0.0f};
    else
        up = up * (1.0f / up_len);

    cam->SetPositionAndTarget(position, focus_, up);
}

ComponentCameraTopDownPtr ComponentCameraTopDown::SetFocusCenter(float3 center)
{
    focus_center_ = center;
    return std::dynamic_pointer_cast<ComponentCameraTopDown>(shared_from_this());
}

ComponentCameraTopDownPtr ComponentCameraTopDown::SetFollowTarget(ObjectPtr target)
{
    follow_ = target;
    if(target)
        follow_name_ = std::string(target->GetName());
    return std::dynamic_pointer_cast<ComponentCameraTopDown>(shared_from_this());
}

ComponentCameraTopDownPtr ComponentCameraTopDown::SetFollowName(std::string_view name)
{
    follow_name_ = std::string(name);
    follow_.reset();
    return std::dynamic_pointer_cast<ComponentCameraTopDown>(shared_from_this());
}

ComponentCameraTopDownPtr ComponentCameraTopDown::SetFollowWeight(float weight)
{
    follow_weight_ = weight;
    return std::dynamic_pointer_cast<ComponentCameraTopDown>(shared_from_this());
}

ComponentCameraTopDownPtr ComponentCameraTopDown::SetHeight(float height)
{
    height_ = height;
    return std::dynamic_pointer_cast<ComponentCameraTopDown>(shared_from_this());
}

ComponentCameraTopDownPtr ComponentCameraTopDown::SetPitch(float pitch)
{
    pitch_ = pitch;
    return std::dynamic_pointer_cast<ComponentCameraTopDown>(shared_from_this());
}

ComponentCameraTopDownPtr ComponentCameraTopDown::SetScreenYaw(float yaw)
{
    screen_yaw_ = yaw;
    return std::dynamic_pointer_cast<ComponentCameraTopDown>(shared_from_this());
}

ComponentCameraTopDownPtr ComponentCameraTopDown::SetSmooth(float speed)
{
    smooth_ = speed;
    return std::dynamic_pointer_cast<ComponentCameraTopDown>(shared_from_this());
}

void ComponentCameraTopDown::GUI()
{
    __super::GUI();

    // GUI内に出現させる
    ImGui::Begin(GetOwner()->GetName().data());
    {
        ImGui::Separator();
        if(ImGui::TreeNode(u8"TopDownCamera")) {
            // GUI上でオーナーから自分を削除します
            if(ImGui::Button(u8"削除"))
                GetOwner()->RemoveComponent(shared_from_this());

            ImGui::DragFloat3(u8"見下ろし中心", &focus_center_.f32[0], 0.1f, -10000.0f, 10000.0f, "%.2f");
            ImGui::DragFloat(u8"カメラ高さ", &height_, 0.1f, 1.0f, 1000.0f, "%.1f");
            ImGui::DragFloat(u8"見下ろし角度", &pitch_, 0.1f, 20.0f, 88.0f, "%.1f");
            ImGui::DragFloat(u8"画面上方向(回旋)", &screen_yaw_, 0.1f, -360.0f, 360.0f, "%.1f");

            // 視線を寄せる対象(将来のキャラクター追従用)
            if(ImGui::BeginCombo(u8"追従対象", follow_name_.data())) {
                auto objs = Scene::GetObjectsPtr<Object>();
                for(int i = 0; i < objs.size(); i++) {
                    auto        obj         = objs[i];
                    std::string object_name = std::string(obj->GetName());
                    if(obj == GetOwnerPtr())
                        continue;

                    bool is_selected = (follow_name_ == object_name);
                    if(ImGui::Selectable(object_name.data(), is_selected)) {
                        follow_name_ = object_name;
                        follow_      = obj;
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::DragFloat(u8"追従割合", &follow_weight_, 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::DragFloat(u8"追従の滑らかさ", &smooth_, 0.1f, 0.0f, 60.0f, "%.1f");

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

CEREAL_REGISTER_TYPE(ComponentCameraTopDown)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentCameraTopDown)
