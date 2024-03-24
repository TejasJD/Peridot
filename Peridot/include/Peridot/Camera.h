#pragma once

#include <memory>
#include <type_traits>

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Peridot {

class Camera {
 public:
  enum Projection { Orthographic, Perspective };

  Camera(const Projection projection, const float zoomOrFov,
         const float aspectRatio)
      : Camera(projection, zoomOrFov, aspectRatio, -100.0f, 100.0f) {}
  Camera(const Projection projection, const float zoomOrFov,
         const float aspectRatio, const float nearPlane, const float farPlane)
      : mProjection(projection),
        mZoomOrFov(zoomOrFov),
        mAspectRatio(aspectRatio),
        mNearPlane(nearPlane),
        mFarPlane(farPlane),
        mPosition(glm::vec3(0.0f)),
        mRotation(glm::vec3(0.0f)) {
    RecalculateViewMatrix();
    RecalculateProjectionMatrix();
  }

  Projection GetProjection() const { return mProjection; }
  float GetZoomOrPov() const { return mZoomOrFov; }
  float GetAspectRatio() const { return mAspectRatio; }
  float GetNearClippingPlane() const { return mNearPlane; }
  float GetFarClippingPlane() const { return mFarPlane; }
  glm::vec3 GetPosition() const { return mPosition; }
  glm::vec3 GetRotation() const { return mRotation; }
  glm::mat4 GetTransform() const { return mTransform; }
  glm::mat4 GetRotationMatrix() const {
    auto inverseTranslation = glm::translate(glm::mat4(1.0f), -mPosition);
    return inverseTranslation * mTransform;
  }
  glm::mat4 GetViewMatrix() const { return glm::inverse(mTransform); }
  glm::mat4 GetProjectionMatrix() const { return mProjMatrix; }

  void SetZoomOrPov(const float zoomOrFov) {
    if (glm::abs(mZoomOrFov - zoomOrFov) < glm::epsilon<float>()) {
      return;
    }
    mZoomOrFov = zoomOrFov;
    RecalculateProjectionMatrix();
  }

  void SetAspectRatio(const float aspectRatio) {
    if (glm::abs(mAspectRatio - aspectRatio) < glm::epsilon<float>()) {
      return;
    }
    mAspectRatio = aspectRatio;
    RecalculateProjectionMatrix();
  }

  void SetNearClippingPlane(const float nearPlane) {
    if (glm::abs(mNearPlane - nearPlane) < glm::epsilon<float>()) {
      return;
    }
    mNearPlane = nearPlane;
    RecalculateProjectionMatrix();
  }

  void SetFarClippingPlane(const float farPlane) {
    if (glm::abs(mFarPlane - farPlane) < glm::epsilon<float>()) {
      return;
    }
    mFarPlane = farPlane;
    RecalculateProjectionMatrix();
  }

  void SetPosition(const glm::vec3& position) {
    mPosition = position;
    RecalculateViewMatrix();
  }

  void SetRotation(const glm::vec3& rotation) {
    mRotation = rotation;
    RecalculateViewMatrix();
  }

  void RecalculateProjectionMatrix() {
    switch (mProjection) {
      case Orthographic:
        mProjMatrix =
            glm::ortho(-(mAspectRatio * mZoomOrFov), mAspectRatio * mZoomOrFov,
                       -1.0f, 1.0f, mNearPlane, mFarPlane);
        break;
      case Perspective:
        mProjMatrix = glm::perspective(mZoomOrFov, mAspectRatio,
                                       std::max(0.1f, mNearPlane), mFarPlane);
        break;
      default:
        break;
    }
  }

  void RecalculateViewMatrix() {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), mPosition);
    glm::mat4 rotationMatrix =
        glm::rotate(glm::mat4(1.0f), mRotation.z, {0.0f, 0.0f, 1.0f}) *
        glm::rotate(glm::mat4(1.0f), mRotation.y, {0.0f, 1.0f, 0.0f}) *
        glm::rotate(glm::mat4(1.0f), mRotation.x, {1.0f, 0.0f, 0.0f});

    mTransform = translationMatrix * rotationMatrix;
  }

 private:
 protected:
  Projection mProjection;
  float mZoomOrFov;
  float mAspectRatio;
  float mNearPlane;
  float mFarPlane;
  glm::mat4 mTransform;
  glm::mat4 mProjMatrix;
  glm::vec3 mPosition;
  glm::vec3 mRotation;
};

}  // namespace Peridot