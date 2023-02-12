#pragma once

const long int ADC_REPORT_TIME = 500;
const long int ADC_DEBOUNCE_TIME = 25;
const int ADC_SAMPLE_COUNT = 6;

typedef struct
{
        int value;
        void (*callback)(void);
} button;

class ADC_buttons {
public:
	void setup_button(const int _value, void (*callback)(void));
        int get_button(const int value);
        void handle_button(long int eventtime, int value);
        void call_button(const int button);
//private:
        button buttons[8];
        int button_count = 0;
        int last_button = -1;
        long int last_pressed = 0;
        long int last_debouncetime = 0;
        int min_value = 0;
        int max_value = 0;
        int sample = 0;
};