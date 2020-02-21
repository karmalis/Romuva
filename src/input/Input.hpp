//
// Created by voltairepunk on 2020-01-14.
//

#ifndef ROMUVA_INPUT_HPP
#define ROMUVA_INPUT_HPP


namespace Romuva {

    namespace Input {

        struct OnKeyAction {
            int key;
            int scancode;
            int action;
            int mods;
        };

        struct OnCharAction {
            unsigned int code;
        };

        struct OnMouseMove {
            double xpos;
            double ypos;
        };

        struct OnMouseEnter {
            bool entered;
        };

        struct OnMouseButtonAction {
            int button;
            int action;
            int mods;
        };

    }

}


#endif //ROMUVA_INPUT_HPP
