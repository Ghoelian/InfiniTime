#include "displayapp/screens/settings/SettingChimeVibration.h"

#include <lvgl/lvgl.h>

#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Styles.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Symbols.h"

using namespace Pinetime::Applications::Screens;

namespace {
  struct Option {
    Pinetime::Controllers::Settings::VibrationStrength vibrationStrength;
    const char* name;
  };

  constexpr std::array<Option, 3> options = {{
    {Pinetime::Controllers::Settings::VibrationStrength::Weak, "Weak"},
    {Pinetime::Controllers::Settings::VibrationStrength::Normal, "Normal"},
    {Pinetime::Controllers::Settings::VibrationStrength::Strong, "Strong"},
  }};

  std::array<CheckboxList::Item, CheckboxList::MaxItems> CreateOptionArray() {
    std::array<Pinetime::Applications::Screens::CheckboxList::Item, CheckboxList::MaxItems> optionArray;
    for (size_t i = 0; i < CheckboxList::MaxItems; i++) {
      if (i >= options.size()) {
        optionArray[i].name = "";
        optionArray[i].enabled = false;
      } else {
        optionArray[i].name = options[i].name;
        optionArray[i].enabled = true;
      }
    }
    return optionArray;
  }

  uint32_t GetDefaultOption(Pinetime::Controllers::Settings::VibrationStrength currentOption) {
    for (size_t i = 0; i < options.size(); i++) {
      if (options[i].vibrationStrength == currentOption) {
        return i;
      }
    }
    return 0;
  }
}

SettingChimeVibration::SettingChimeVibration(Pinetime::Controllers::Settings& settingsController,
                                             Pinetime::Controllers::MotorController& motorController)
  : checkboxList(
      0,
      1,
      "Chime strength",
      Symbols::tachometer,
      GetDefaultOption(settingsController.GetChimeVibration()),
      [&settings = settingsController, &motor = motorController](uint32_t index) {
        // Preview current setting
        motor.RunForDuration(static_cast<uint8_t>(options[index].vibrationStrength));

        settings.SetChimeVibration(options[index].vibrationStrength);
        settings.SaveSettings();
      },
      CreateOptionArray()) {
}

SettingChimeVibration::~SettingChimeVibration() {
  lv_obj_clean(lv_scr_act());
}