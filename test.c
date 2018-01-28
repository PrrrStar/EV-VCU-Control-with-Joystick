#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <linux/joystick.h>
#include <linux/can.h>
#include <linux/can/raw.h>


/**
 * Reads a joystick event from the joystick device.
 *
 * Returns 0 on success. Otherwise -1 is returned.
 */
int read_event(int fd, struct js_event *event)
{
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    /* Error, could not read full event. */
    return -1;
}

/**
 * Returns the number of axes on the controller or 0 if an error occurs.
 */
size_t get_axis_count(int fd)
{
    __u8 axes;

    if (ioctl(fd, JSIOCGAXES, &axes) == -1)
        return 0;

    return axes;
}

/**
 * Returns the number of buttons on the controller or 0 if an error occurs.
 */
size_t get_button_count(int fd)
{
    __u8 buttons;
    if (ioctl(fd, JSIOCGBUTTONS, &buttons) == -1)
        return 0;

    return buttons;
}

/**
 * Current state of an axis.
 */
struct axis_state {
    short x, y;
};

/**
 * Keeps track of the current axis state.
 *
 * NOTE: This function assumes that axes are numbered starting from 0, and that
 * the X axis is an even number, and the Y axis is an odd number. However, this
 * is usually a safe assumption.
 *
 * Returns the axis that the event indicated.
 */
size_t get_axis_state(struct js_event *event, struct axis_state axes[3])
{
    size_t axis = event->number / 2;

    if (axis < 3)
    {
        if (event->number % 2 == 0)
            axes[axis].x = event->value;
        else
            axes[axis].y = event->value;
    }

    return axis;
}

int main(int argc, char *argv[]) {

        const char *device;
        int js;
        struct js_event event;
        struct axis_state axes[3] = {0};
        size_t axis;

        bool bw_switch = false;
        int speed = 0;

        //default (Forward) : 90 degree
        int angle = 90;


        if (argc > 1)
                device = argv[1];
        else
                device = "/dev/input/js0";

        js = open(device, O_RDONLY);

        if (js == -1)
                perror("Could not open joystick");

        while (read_event(js, &event) == 0) {
                switch (event.type) {

                        case JS_EVENT_BUTTON:
            //    printf("Button %u %s\n", event.number, event.value ? "pressed" : "released");


                                if (event.number == 0){
                                        printf("Green : A Button");
                                }

                                if (event.number == 1){
                                        printf("Red : B Button");
                                }

                                if (event.number == 2){
                                        printf("Blue : X Button");
                                }

                                if (event.number == 3){
                                        printf("Yellow : Y Button");
                                }

                                if (event.number == 4){
                                        printf("Left Lamp");
                                }

                                if (event.number == 5){
                                        printf("Rigth Lamp");
                                }

                                if (event.number == 6){
                                        if (event.value == true){
                                                bw_switch = true;
                                                printf("Backward");
                                        }else if (event.value 
                                        printf("Forward",event.value);

                                }

                                if (event.number == 7){
                                        printf("Remote ON",event.value);

                                }
                                printf("\n");

                        break;


                        case JS_EVENT_AXIS:
                                axis = get_axis_state(&event, axes);
                                if (axis < 3){
                        //printf("Axis %zu at (%6d, %6d)\n", axis, axes[axis].x, axes[axis].y);
                                        if (axis==0&&axes[axis].x == 0&&axes[axis].y == -32767){
                                                printf("↑");

                                        }
                                        if (axis==0&&axes[axis].x == 0&&axes[axis].y == 32767){
                                                printf("↓");

                                        }
                                        if (axis==0&&axes[axis].x == -32767&&axes[axis].y == -2){
                                                printf("←");

                                        }
                                        if (axis==0&&axes[axis].x == 32767&&axes[axis].y == -2){
                                                printf("→");

                                        }
                                        printf("\n");

                                }
                        break;

                        default:
                /* Ignore init events. */
                break;
                }
        
        fflush(stdout);
        }

        close(js);
        return 0;
}
