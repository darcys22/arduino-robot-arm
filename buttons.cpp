// https://github.com/Klipper3d/klipper/pull/977/files

#include "buttons.h"
#include <Arduino.h>

void ADC_buttons::setup_button(const int _value, void (*callback)(void))
{
    if (min_value == 0)
        min_value = _value - 20;
    else
        min_value = min(min_value, _value - 20);

    if (max_value == 0)
        max_value = _value + 20;
    else
        max_value = max(max_value, _value + 20);

    buttons[button_count] = button{_value, callback};
    button_count = button_count + 1;
}

int ADC_buttons::get_button(const int value)
{
    int best_button = -1;
    int best_distance = INT16_MAX;
    int current_distance = INT16_MAX;
    if ((min_value != 0) && (max_value != 0) && (value >= min_value) && (value <= max_value))
        for (int i = 0; i < button_count; i++)
        {   
            current_distance = abs(value - buttons[i].value);
            if (current_distance < best_distance)
            {
                best_button = i;
                best_distance = current_distance;
            }
        }
    return best_button;
}


void ADC_buttons::handle_button(long int eventtime, int value)
{
    auto btn = get_button(value);

    if (btn == -1)
        return;

    // If the button changed, due to noise or pressing:
    if (btn != last_button)
    {
        // reset the debouncing timer
        last_debouncetime = eventtime;
        sample = 0;
    }
    else
    {
        sample++;
    }

    // button debounce check & new button pressed
    if ((eventtime - last_debouncetime) >= ADC_DEBOUNCE_TIME 
        && (eventtime - last_pressed) >= ADC_REPORT_TIME
        && last_button == btn 
        && sample >=  ADC_SAMPLE_COUNT)
    {
        call_button(btn);
        last_pressed = eventtime;
    }

    last_button = btn;
}

void ADC_buttons::call_button(const int button)
{
    if (button != -1 && button < button_count)
        buttons[button].callback();
}
