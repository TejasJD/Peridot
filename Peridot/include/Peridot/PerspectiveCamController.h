#pragma once

#include "Peridot/Camera.h"

namespace Peridot {

class PerspectiveCameraController {
 public:
  PerspectiveCameraController(const float delta, const float translationSpeed,
                              const float rotationSpeed)
      : mCamera(Camera::Perspective, 1.0f, glm::radians(45.0f)),
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

  void MoveWithCursor(double prevX, double prevY, double newX, double newY) {
    auto deltaX = prevX - newX;
    auto deltaY = prevY - newY;
    glm::vec3 currentRotation = mCamera.GetRotation();
    currentRotation.y += deltaX * mSensitivity * mDelta;
    currentRotation.x += deltaY * mSensitivity * mDelta;
    mCamera.SetRotation(currentRotation);
  }

  void MoveForward() {
    glm::vec3 currentPosition = mCamera.GetPosition();
    glm::vec3 currentDirection = mCamera.GetRotationMatrix()[2];
    currentPosition -= currentDirection * mTranslationSpeed * mDelta;
    mCamera.SetPosition(currentPosition);
  }

  void MoveBack() {
    glm::vec3 currentPosition = mCamera.GetPosition();
    glm::vec3 currentDirection = mCamera.GetRotationMatrix()[2];
    currentPosition += currentDirection * mTranslationSpeed * mDelta;
    mCamera.SetPosition(currentPosition);
  }

  void MoveRight() {
    glm::vec3 currentPosition = mCamera.GetPosition();
    glm::vec3 currentDirection = mCamera.GetRotationMatrix()[0];
    currentPosition += currentDirection * mTranslationSpeed * mDelta;
    mCamera.SetPosition(currentPosition);
  }

  void MoveLeft() {
    glm::vec3 currentPosition = mCamera.GetPosition();
    glm::vec3 currentDirection = mCamera.GetRotationMatrix()[0];
    currentPosition -= currentDirection * mTranslationSpeed * mDelta;
    mCamera.SetPosition(currentPosition);
  }

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

  void RotateLeft() {
    glm::vec3 currentRotation = mCamera.GetRotation();
    currentRotation.y -= mRotationSpeed * mDelta;
    mCamera.SetRotation(currentRotation);
  }

  void RotateRight() {
    glm::vec3 currentRotation = mCamera.GetRotation();
    currentRotation.y += mRotationSpeed * mDelta;
    mCamera.SetRotation(currentRotation);
  }

 private:
  Camera mCamera;
  float mDelta;
  float mTranslationSpeed;
  float mRotationSpeed;
  float mSensitivity = 1.0f;
};

}  // namespace Peridot