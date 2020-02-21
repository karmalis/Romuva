//
// Created by voltairepunk on 2019-12-30.
//


#include "ECS.hpp"
#include "Manager.hpp"

namespace Romuva {

    namespace ECS {

        void ECS::Entity::addGroup(Group group) {
            this->_groupBitSet[group] = true;
            this->_manager.addToGroup(this, group);
        }
    }

}
