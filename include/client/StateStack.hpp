#ifndef _STATE_STACK_CLIENT_HPP
#define _STATE_STACK_CLIENT_HPP

#include <stack>
#include <memory>
#include <SFML/System.hpp>

//Forward declarations
class AbstractState;

/// States manager that holds all the states and manages them.
/// \see State
class StateStack : private sf::NonCopyable
{
    public:
        /// Destructor.
        /// This declarations is here in order to avoid a warning about inlining.
        ~StateStack() = default;

        /// Call the display() function of the top state.
        void display();

        /// Call the handleEvent() function of the top state.
        void handleEvent();

        /// Add a new state of the template type to the top of the stack.
        /// \tparam StateType The type of the state to construct.
        template <typename StateType>
        void push();

        /// Delete the top state.
        void pop();

        /// Delete all the states.
        void clear();

        /// Check if the stack is empty or not.
        /// \return True if the stack is empty, false otherwise.
        bool isEmpty() const;

    private:
        std::stack<std::unique_ptr<AbstractState>> _stack;
};

template <typename StateType>
void StateStack::push()
{
    _stack.emplace(new StateType(*this));
}

#endif// _STATE_STACK_CLIENT_HPP

