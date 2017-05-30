#include "signals.hpp"
#include <iostream>

// a handler are a kind of slot,, and as an implementation detail require usage of the
// "curiously redcurring rwmplate pattern". That is the intention is for instances of
// a class C that need to do something in reaction to a signal firing to have an is-a
// relationship with a slot that is parametrized on class C.

class CharacterHandler : public Slot<CharacterHandler>
{
public:
	void HandleCharacter(char c)
	{
		std::cout << "The user entered '" << c << "'" << std::endl;
	}
};

class DigitHandler : public Slot<DigitHandler>
{
public:
	void HandleCharacter(char c)
	{
		if (c >= '0' && c <= '9') {
			int n = static_cast<int>(c - '0');
			std::cout << "  " << n << " * " << n << " = " << n*n << std::endl;
		}
	}
};

int main()
{
	bool done = false;

	char c;
	Signal<char> signal;
	CharacterHandler character_handler;
	DigitHandler digit_handler;

	// can attach a signal to a slot with matching arguments
	signal.connect(character_handler, &CharacterHandler::HandleCharacter);

	// can also attach a lambda, associated with a slot.
	// (the lambda could capture the slot and use it like anything else it captures
	// however technically all the associated slot is doing is allowing you to have
	// a way of disconnecting the lambda e.g. in this case signal.disconnect(characterHandler)
	signal.connect(character_handler,
		[&](char c) -> void {
			if (c == 'q')
				done = true;
		}
	);

	// can also attach a slot to a signal ... this means the same as the above.
	digit_handler.connect(signal, &DigitHandler::HandleCharacter);

	do
	{
		std::cin >> c;
		signal.fire(c);

	} while (! done);
	
	// can disconnect like this
	character_handler.disconnect(signal);

	// or this
	signal.disconnect(digit_handler);

	//although disconnecting wasnt necessary here in that just gaving everything go out of scope
	// wouldve done the right thing.

    return 0;
}

