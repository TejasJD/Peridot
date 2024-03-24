#pragma once

#include "Peridot/Camera.h"

namespace Peridot {

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