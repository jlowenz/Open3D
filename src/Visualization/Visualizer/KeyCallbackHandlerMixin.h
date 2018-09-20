#pragma once

#include <functional>
#include <map>
#include <string>
#include <Visualization/Visualizer/Visualizer.h>

namespace open3d {

typedef std::function<bool(Visualizer *)> VisualizerCallback;

struct CallbackRecord {
    CallbackRecord() {}
    CallbackRecord(int k, VisualizerCallback cb)
        : key(k), callback(cb), description("None") {}
    CallbackRecord(int k, VisualizerCallback cb, const std::string &desc)
        : key(k), callback(cb), description(desc) {}
    CallbackRecord(const CallbackRecord &o)
        : key(o.key), callback(o.callback), description(o.description) {}
    ~CallbackRecord() {}
    int key;
    VisualizerCallback callback;
    std::string description;
};

template <typename V>
class KeyCallbackHandlerMixin : public V {
   public:
    void RegisterKeyCallback(int key, VisualizerCallback cb,
                             const std::string &description = "");

   protected:
    typedef std::pair<int, CallbackRecord> KeyCallbackPair;
    std::map<int, CallbackRecord> key_to_callback_;
    virtual void KeyPressCallback(GLFWwindow *window, int key, int scancode,
                                  int action, int mods) override;
    std::string PrintKeyToString(int key);
};

template <typename V>
void KeyCallbackHandlerMixin<V>::RegisterKeyCallback(
    int key, VisualizerCallback callback, const std::string &desc) {
    CallbackRecord cr(key, callback, desc);
    key_to_callback_[key] = cr;
}

template <typename V>
void KeyCallbackHandlerMixin<V>::KeyPressCallback(GLFWwindow *window, int key,
                                                  int scancode, int action,
                                                  int mods) {
    if (action == GLFW_RELEASE) {
        // TODO: this should be fixed to either only handle on RELEASE, or to make the callbacks responsible
        V::KeyPressCallback(window, key, scancode, action, mods);
        return;
    }
    auto cbrec = key_to_callback_.find(key);
    if (cbrec != key_to_callback_.end()) {
        if (cbrec->second.callback(dynamic_cast<V *>(this))) {
            V::UpdateGeometry();
        }
        V::UpdateRender();
    } else {
        V::KeyPressCallback(window, key, scancode, action, mods);
    }
}

template <typename V>
std::string KeyCallbackHandlerMixin<V>::PrintKeyToString(int key) {
    if (key == GLFW_KEY_SPACE) {  // 32
        return std::string("Space");
    } else if (key >= 39 && key <= 96) {  // 39 - 96
        return std::string(1, char(key));
    } else if (key == GLFW_KEY_ESCAPE) {  // 256
        return std::string("Esc");
    } else if (key == GLFW_KEY_ENTER) {  // 257
        return std::string("Enter");
    } else if (key == GLFW_KEY_TAB) {  // 258
        return std::string("Tab");
    } else if (key == GLFW_KEY_BACKSPACE) {  // 259
        return std::string("Backspace");
    } else if (key == GLFW_KEY_INSERT) {  // 260
        return std::string("Insert");
    } else if (key == GLFW_KEY_DELETE) {  // 261
        return std::string("Delete");
    } else if (key == GLFW_KEY_RIGHT) {  // 262
        return std::string("Right arrow");
    } else if (key == GLFW_KEY_LEFT) {  // 263
        return std::string("Left arrow");
    } else if (key == GLFW_KEY_DOWN) {  // 264
        return std::string("Down arrow");
    } else if (key == GLFW_KEY_UP) {  // 265
        return std::string("Up arrow");
    } else if (key == GLFW_KEY_PAGE_UP) {  // 266
        return std::string("Page up");
    } else if (key == GLFW_KEY_PAGE_DOWN) {  // 267
        return std::string("Page down");
    } else if (key == GLFW_KEY_HOME) {  // 268
        return std::string("Home");
    } else if (key == GLFW_KEY_END) {  // 269
        return std::string("End");
    } else if (key == GLFW_KEY_CAPS_LOCK) {  // 280
        return std::string("Caps lock");
    } else if (key == GLFW_KEY_SCROLL_LOCK) {  // 281
        return std::string("Scroll lock");
    } else if (key == GLFW_KEY_NUM_LOCK) {  // 282
        return std::string("Num lock");
    } else if (key == GLFW_KEY_PRINT_SCREEN) {  // 283
        return std::string("PrtScn");
    } else if (key == GLFW_KEY_PAUSE) {  // 284
        return std::string("Pause");
    } else if (key >= 290 && key <= 314) {  // 290 - 314
        return std::string("F") + std::to_string(key - 289);
    }
    return std::string("Unknown");
}

}  // namespace open3d