# Transform

```cpp
class Transform {
public:
    GameObject* gameObject = nullptr;
    Transform* parent = nullptr;
    std::vector<Transform*> children;

    Transform(const Transform& transform);
    Transform(glm::vec3 position = glm::vec3(1), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
    // 设置position
    void SetPosition(const glm::vec3& position);
    // 设置rotation
    void SetRotation(const glm::vec3& rotation);
    // 设置scale
    void SetScale(const glm::vec3& scale);
    // 设置parent节点
    void SetParent(Transform* parent);
    void RemoveParent();
    // 向direction方向移动distance
    void Translate(const glm::vec3& direction, float distance);
    // X轴旋转角增加pitch[角度]
    void RotateX(float pitch, bool limit = false, float lower = -89, float upper = 89);
    // Y轴旋转角增加yaw[角度]
    void RotateY(float yaw);
    // Z轴旋转角增加roll[角度]
    void RotateZ(float roll);
    glm::vec3 GetPosition() const;
    glm::vec3 GetRotation() const;
    glm::vec3 GetScale() const;
    glm::vec3 GetAxisX();
    glm::vec3 GetAxisY();
    glm::vec3 GetAxisZ();
    glm::mat4 GetModelMatrix();
    glm::mat4 GetTranslateMatrix();
    glm::mat4 GetRotationMatrix();
    glm::mat4 GetScaleMatrix();
};
```

## 注意事项

- `GameObject`通过`Transform`形成树形结构。
- `Transform`的`model`矩阵等不会在修改`position, rotation, scale`之后马上更新，所以应该使用`Get*()`函数来获取，而不是直接访问`modelMatrix`成员。同样地，对`position`等成员的修改也应使用`Set*()`函数而不是直接操作成员。
- `Transform`的变换是相对于`local space`的。当存在父子关系时，先辈的`position`和`rotation`会依次作用到子节点上。  
  例如：`a->b->c`(a是b的父节点，b是c的父节点)  
  Then `c.modelMatrix = (a.translateMat * a. rotationMat) * (b.translateMat * b. rotationMat) * (c.translateMat * c. rotationMat * c.scaleMat)`
