// -*- indent-tabs-mode: nil; -*-

#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "base.hpp"
#include "bridge.h"

// http://developer.apple.com/documentation/Darwin/Conceptual/KEXTConcept/KEXTConceptIOKit/hello_iokit.html#//apple_ref/doc/uid/20002366-CIHECHHE
class org_pqrs_driver_PCKeyboardHack : public IOService
{
  OSDeclareDefaultStructors(org_pqrs_driver_PCKeyboardHack);

public:
  virtual bool init(OSDictionary* dictionary = 0);
  virtual void free(void);
  virtual IOService* probe(IOService* provider, SInt32* score);
  virtual bool start(IOService* provider);
  virtual void stop(IOService* provider);

  static void customizeAllKeymap(void);

private:
  // see IOHIDUsageTables.h or http://www2d.biglobe.ne.jp/~msyk/keyboard/layout/usbkeycode.html
  class KeyMapIndex {
  public:
    enum Value {
      NONE     = 0, // NONE must be a unique value in this enum.
      CAPSLOCK = 0x39,
      JIS_KANA = 0x88,
      JIS_NFER = 0x8b,
      JIS_XFER = 0x8a,
    };
    static Value bridgeKeyindexToValue(int bridgeKeyIndex) {
      switch (bridgeKeyIndex) {
        case BRIDGE_KEY_INDEX_CAPSLOCK: return CAPSLOCK;
        case BRIDGE_KEY_INDEX_JIS_KANA: return JIS_KANA;
        case BRIDGE_KEY_INDEX_JIS_NFER: return JIS_NFER;
        case BRIDGE_KEY_INDEX_JIS_XFER: return JIS_XFER;
      }
      return NONE;
    }
  };
  enum {
    MAXNUM_KEYBOARD = 16,
  };

  // ------------------------------------------------------------
  struct HookedKeyboard {
    IOHIKeyboard* kbd;

    unsigned int originalKeyCode[BRIDGE_KEY_INDEX__END__];

    void initialize(IOHIKeyboard* p);
    void terminate(void);
    void refresh(void);
  };
  static HookedKeyboard hookedKeyboard[MAXNUM_KEYBOARD];

  // ------------------------------------------------------------
  static HookedKeyboard* new_hookedKeyboard(void);
  static HookedKeyboard* search_hookedKeyboard(const IOHIKeyboard* kbd);

  static bool notifierfunc_hookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier);
  static bool notifierfunc_unhookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier);

  static bool isTargetDevice(IOHIKeyboard* kbd);

  static bool customizeKeyMap(IOHIKeyboard* kbd);
  static bool restoreKeyMap(IOHIKeyboard* kbd);

  static BridgeUserClientStruct configuration_;

  IONotifier* keyboardNotifier;
  IONotifier* terminatedNotifier;
};

#endif