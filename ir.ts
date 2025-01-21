enum PIN {
    P0 = 3,
    P1 = 2,
    P2 = 1,
    P8 = 18,
    //P9 = 10,
    P12 = 20,
    P13 = 23,
    P14 = 14,
    P15 = 15,
    p16=  24,
};

/**
 * Custom blocks
 */
//% weight=100 color=#0fbc11 icon="\uf11b" block="IR"
namespace IR {
    let state: number;
    let data1: number;
    let irstate: number;
    let irData: number = -1;
    let pinNumber: number;

    //% shim=DFRobotIR::irCode
    function irCode(pin :number): number {
        return 0;
    }

    /**
     * Read infrared value
     */
    
    //% weight=60
    //% block="read IR key value"
    export function IR_read(): number {
        pins.setPull(DigitalPin.P14, PinPullMode.PullUp)
        return valuotokeyConversion();
    }

    /**
     * The callback function when infrared information is received
     */

    //% weight=50
    //% block="IR on Pin %T received"
    //% draggableParameters
    export function IR_callbackUser(T: PIN, cb: (message: number) => void) {
        let _T;
        _T = DigitalPin.P14;
        pinNumber = T;
        switch (T) {
            case PIN.P0: _T = DigitalPin.P0; break;
            case PIN.P1: _T = DigitalPin.P1; break;
            case PIN.P2: _T = DigitalPin.P2; break;
            case PIN.P8: _T = DigitalPin.P8; break;
            case PIN.P12: _T = DigitalPin.P12; break;
            // case PIN.P10: _T = DigitalPin.P10; break;
            case PIN.P13: _T = DigitalPin.P13; break;
            case PIN.P14: _T = DigitalPin.P14; pinNumber = 14; break;
            case PIN.P15: _T = DigitalPin.P15; break;
            default: _T = DigitalPin.P14; break;
        }

        pins.setPull(_T, PinPullMode.PullUp)
        state = 1;
        control.onEvent(11, 22, function () {
            cb(data1)

        })
    }

    basic.forever(() => {
        if (state == 1) {
            irstate = irCode(pinNumber);
            if (irstate != 0) {
                data1 = irstate & 0xff;
                control.raiseEvent(11, 22)
            }
        }

        basic.pause(50);
    })

    function valuotokeyConversion(): number {
        //serial.writeValue("x", irCode() )
        let data = irCode(pinNumber);
        if (data == 0) {
        } else {
            irData = data & 0xff;
        }
        return irData;
    }
}
