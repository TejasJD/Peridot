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
  glm::mat4 GetViewMatrix() const { return mViewMatrix; }
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
        mProjMatrix =
            glm::perspective(mZoomOrFov, mAspectRatio, mNearPlane, mFarPlane);
        break;
      default:
        break;
    }
  }

  void RecalculateViewMatrix() {
    glm::vec4 lookingAt = {0.0f, 0.0f, -1.0f, 1.0f};
    glm::vec4 cameraPosition = {0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec4 cameraUp = {0.0f, 1.0f, 0.0f, 1.0f};
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), mPosition);
    glm::mat4 rotationMatrix =
        glm::rotate(glm::mat4(1.0f), mRotation.z, {0.0f, 0.0f, 1.0f}) *
        glm::rotate(glm::mat4(1.0f), mRotation.y, {0.0f, 1.0f, 0.0f}) *
        glm::rotate(glm::mat4(1.0f), mRotation.x, {1.0f, 0.0f, 0.0f});

    lookingAt = translationMatrix * rotationMatrix * lookingAt;
    cameraPosition = translationMatrix * rotationMatrix * cameraPosition;
    cameraUp = rotationMatrix * cameraUp;

    mViewMatrix = glm::lookAt(
        glm::vec3{cameraPosition.x, cameraPosition.y, cameraPosition.z},
        glm::vec3{lookingAt.x, lookingAt.y, lookingAt.z},
        {cameraUp.x, cameraUp.y, cameraUp.z});
  }

 private:
 protected:
  Projection mProjection;
  float mZoomOrFov;
  float mAspectRatio;
  float mNearPlane;
  float mFarPlane;
  glm::mat4 mViewMatrix;
  glm::mat4 mProjMatrix;
  glm::vec3 mPosition;
  glm::vec3 mRotation;
};

class OrthographicCameraController {
 public:
  OrthographicCameraController(const float delta, const float translationSpeed,
                               const float rotationSpeed)
      : mCamera(Camera::Orthographic, 1.0f, 1.0f),
        mDelta(delta),
        mTranslationSpeed(translationSpeed),
        mRotationSpeed(rotationSpeed) {}

  Camera& GetCamera() { return mCamera; }
  const Camera& GetCamera() const { return mCamera; }
  float GetTranslationSpeed() const { return mTranslationSpeed; }
  float GetRotationSpeed() const { return mRotationSpeed; }

  void SetTranslationSpeed(const float speed) { mTranslationSpeed = speed; }
  void SetRotationSpeed(const float speed) { mRotationSpeed = speed; }
  void SetDelta(const float delta) { mDelta = delta; }

  void MoveUp() {
    glm::vec3 currentPosition = mCamera.GetPosition();
    currentPosition.y += mTranslationSpeed * mDelta;
    mCamera.SetPosition(currentPosition);
  }

  void MoveDown() {
    glm::vec3 currentPosition = mCamera.GetPosition();
    currentPosition.y -= mTranslationSpeed * mDelta;
    mCamera.SetPosition(currentPosition);
  }

  void MoveRight() {
    glm::vec3 currentPosition = mCamera.GetPosition();
    currentPosition.x += mTranslationSpeed * mDelta;
    mCamera.SetPosition(currentPosition);
  }

  void MoveLeft() {
    glm::vec3 currentPosition = mCamera.GetPosition();
    currentPosition.x -= mTranslationSpeed * mDelta;
    mCamera.SetPosition(currentPosition);
  }

  void RotateLeft() {
    glm::vec3 currentRotation = mCamera.GetRotation();
    currentRotation.z -= mRotationSpeed * mDelta;
    mCamera.SetRotation(currentRotation);
  }

  void RotateRight() {
    glm::vec3 currentRotation = mCamera.GetRotation();
    currentRotation.z += mRotationSpeed * mDelta;
    mCamera.SetRotation(currentRotation);
  }

 private:
  Camera mCamera;
  float mDelta;
  float mTranslationSpeed;
  float mRotationSpeed;
};

}  // namespace Peridot