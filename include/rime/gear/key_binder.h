//
// Copyleft 2011 RIME Developers
// License: GPLv3
//
// 2011-11-23 GONG Chen <chen.sst@gmail.com>
//
#ifndef RIME_KEY_BINDER_H_
#define RIME_KEY_BINDER_H_

#include <rime/common.h>
#include <rime/component.h>
#include <rime/processor.h>

namespace rime {

class KeyBindings;

class KeyBinder : public Processor {
 public:
  KeyBinder(Engine *engine);
  virtual Result ProcessKeyEvent(const KeyEvent &key_event);

 protected:
  bool ReinterpretPagingKey(const KeyEvent &key_event);
  
  scoped_ptr<KeyBindings> key_bindings_;
  bool redirecting_;
  int last_key_;
};

}  // namespace rime

#endif  // RIME_KEY_BINDER_H_
