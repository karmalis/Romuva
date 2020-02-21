//
// Created by voltairepunk on 2019-12-30.
//

#ifndef ROMUVA_MODULE_HPP
#define ROMUVA_MODULE_HPP

#include <string>

namespace Romuva {

    namespace Core {

        class Engine;

        class Module {

        public:

            Module(std::string name, bool repeating, bool background);

            virtual ~Module() {}

            virtual bool init();

            virtual void postInit();

            virtual void update(double delta);

            virtual void shutdown();

            const std::string &getName() { return this->_name; }

        private:

            Module(const Module& copy) = delete;
            const Module& operator= (const Module&) = delete;

        protected:

            Module() {}

            friend class Engine;

            Engine* _engine;
            std::string _name;

            bool _repeating;
            bool _background;

        };

    }

}


#endif //ROMUVA_MODULE_HPP
