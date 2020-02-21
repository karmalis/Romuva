//
// Created by voltairepunk on 2019-12-30.
//

#ifndef ROMUVA_ECS_HPP
#define ROMUVA_ECS_HPP

#include <array>
#include <vector>
#include <bitset>
#include <memory>
#include "../log/Logger.hpp"

namespace Romuva {

    namespace ECS {

        class Component;

        class Entity;

        class Manager;

        using ComponentID = std::size_t;
        using Group = std::size_t;

        inline ComponentID getNewComponentTypeID() {
            static ComponentID lastID = 0u;
            return lastID++;
        }

        template<typename T>
        inline ComponentID getComponentTypeId() noexcept {
            static_assert(std::is_base_of<Component, T>::value, "");
            static ComponentID typeID = getNewComponentTypeID();
            return typeID;
        }

        constexpr std::size_t maxComponents = 32;
        constexpr std::size_t maxGroups = 32;

        using ComponentBitSet = std::bitset<maxComponents>;
        using GroupBitSet = std::bitset<maxGroups>;

        using ComponentArray = std::array<Component *, maxComponents>;

        class Component {
        public:

            Entity * entity;

            Component() : entity(nullptr), _dirty(true) {}

            virtual bool init() { return true; }
            virtual void update(double delta) { if (_dirty) { clean(); } }
            virtual void draw() {}
            virtual void clean() { _dirty = false; }
            virtual ~Component() = default;

        protected:

            bool _dirty;
        };

        class Entity {
        private:

            Manager& _manager;
            bool _active = true;
            std::vector<std::unique_ptr<Component>> _components;

            ComponentArray _componentArray;
            ComponentBitSet _componentBitSet;
            GroupBitSet _groupBitSet;

        public:

            explicit Entity(Manager& manager) : _componentArray{}, _manager(manager) {}

            Entity(const Entity& copy) = delete;

            void update(double delta) {
                for (auto& c : this->_components) c->update(delta);
            }

            void draw() {
                for (auto& c : this->_components) c->draw();
            }

            bool isActive() const { return this->_active; }
            void destroy() { this->_active = false; }

            bool hasGroup(Group group) {
                return this->_groupBitSet[group];
            }

            void addGroup(Group group);

            void delGroup(Group group) {
                this->_groupBitSet[group] = false;
            }

            template<typename T>
            bool hasComponent() const {
                return this->_componentBitSet[getComponentTypeId<T>()];
            }

            template<typename T, typename... TArgs>
            T& addComponent(TArgs&&... args) {
                T* c(new T(std::forward<TArgs>(args)...));
                c->entity = this;
                std::unique_ptr<Component> uPtr { c };
                this->_components.emplace_back(std::move(uPtr));
                this->_componentArray[getComponentTypeId<T>()] = c;
                this->_componentBitSet[getComponentTypeId<T>()] = true;

                if (!c->init()) {
                    gLogError << "Could not initialise component";
                }

                return *c;
            }

            template<typename T>
            T& getComponent() const {
                auto ptr(this->_componentArray[getComponentTypeId<T>()]);
                return *static_cast<T*>(ptr);
            }
        };

    }

}

#endif //ROMUVA_ECS_HPP
