#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

#include "Peridot/Input.h"

namespace Peridot {

ButtonState PollModeInput::GetKeyState(const KeyCode key) const {
  ButtonState buttonState = ButtonState::Released;
  int32_t state = glfwGetKey(mContext->GetRawWindow(), (int32_t)key);
  if (state == GLFW_PRESS) {
    buttonState = ButtonState::Pressed;
  } else if (state == GLFW_RELEASE) {
    buttonState = ButtonState::Released;
  }

  return buttonState;
}

ButtonState PollModeInput::GetMouseButtonState(const MouseCode button) const {
  ButtonState buttonState = ButtonState::Released;
  int32_t state = glfwGetMouseButton(mContext->GetRawWindow(), (int32_t)button);
  if (state == GLFW_PRESS) {
    buttonState = ButtonState::Pressed;
  } else if (state == GLFW_RELEASE) {
    buttonState = ButtonState::Released;
  }

  return buttonState;
}

std::pair<double, double> PollModeInput::GetCursorPos() const {
  std::pair<double, double> cursorPositions;
  glfwGetCursorPos(mContext->GetRawWindow(), &cursorPositions.first,
                   &cursorPositions.second);
  return cursorPositions;
}

void PollModeInput::RegisterKeyCallback(const KeyCode key,
                                        const ButtonStateCallback& callback) {
  spdlog::trace(__FUNCTION__);

  if (mKeyMappings.find(key) != mKeyMappings.end()) {
    spdlog::warn("Callback already registered for key");
    return;
  }
  spdlog::info("Registered callback");
  mKeyMappings[key] = callback;
}

void PollModeInput::RegisterMouseCallback(const MouseCode button,
                                          const ButtonStateCallback& callback) {
  spdlog::trace(__FUNCTION__);
  if (mMouseButtonMappings.find(button) != mMouseButtonMappings.end()) {
    spdlog::warn("Callback already registered for mouseButon: '{}'",
                 (int32_t)button);
    return;
  }

  spdlog::info("Registered callback for mouse button: '{}'", (int32_t)button);
  mMouseButtonMappings[button] = callback;
}

void PollModeInput::UnregisterKeyCallback(const KeyCode key) {
  int scanCode = glfwGetKeyScancode((int32_t)key);
  const char* keyName = glfwGetKeyName((int32_t)key, scanCode);

  if (mKeyMappings.find(key) == mKeyMappings.end()) {
    spdlog::warn("no callback assigned");
    return;
  }

  spdlog::info("unregistered callback");
  mKeyMappings.erase(key);
}

void PollModeInput::UnregisterMouseCallback(const MouseCode button) {
  if (mMouseButtonMappings.find(button) == mMouseButtonMappings.end()) {
    spdlog::warn("Callback not registered for mouseButon: '{}'",
                 (int32_t)button);
    return;
  }
  mMouseButtonMappings.erase(button);
}

void PollModeInput::RegisterCursorCallback(const CursorCallback& callback) {
  spdlog::trace(__FUNCTION__);
  if (mCursorCallback) {
    return;
  }
  mCursorCallback = std::make_shared<CursorCallback>(callback);
}

void PollModeInput::UnregisterCursorCallback() {
  spdlog::trace(__FUNCTION__);
  if (!mCursorCallback) {
    return;
  }
  mCursorCallback = nullptr;
}

}  // namespace Peridot