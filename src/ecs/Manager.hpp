//
// Created by voltairepunk on 2019-12-30.
//

#ifndef ROMUVA_MANAGER_HPP
#define ROMUVA_MANAGER_HPP

#include "../Module.hpp"
#include "ECS.hpp"

namespace Romuva {

    namespace ECS {

    class Manager : public Core::Module {
        private:

            std::vector<std::unique_ptr<Entity>> _entities;
            std::array<std::vector<Entity*>, maxGroups> _groupedEntities;

        public:

            Manager();
            ~Manager() = default;

            bool init() override;
            void postInit() override;
            void update(double delta) override;
            void shutdown() override;

            void draw();
            void refresh();
            void addToGroup(Entity* entity, Group group);

            std::vector<Entity*>& getGroup(Group group);

            Entity& addEntity();
        };

    }

}


#endif //ROMUVA_MANAGER_HPP
