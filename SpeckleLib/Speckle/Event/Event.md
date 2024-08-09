
# Event

## Contents
1. [Purpose](#purpose)
2. [Overview](#over)
3. [Detail](#implement)
4. [Example](#example)
	- [App](#app)
	- [Button](#button)
	- [Calculation](#calc)
	- [Calculator](#calculator)

## Purpose <a name="purpose"></a>

The Event module provides tools to prevent coupled software components through a publish/subscribe interface. The following scenarios outline some use cases:

- User interactions with an interface should typically be handled by some business logic within an app. Because an action on one component should lead to a response by another, the obvious approach is to directly couple the business logic with a UI control. This approach rapidly becomes unmanageble as:
	- Reusability decreases as code which should focus on single, simple operations (e.g. a UI button) is increasing coupled to other, divergent tasks.
	- Complexity increases exponentially as the number of possible combinations of the UI and responses inevitably expands over time
- Object interactions may require detailed solutions, e.g. the shape and components for a structural joint where two or more steel framing objects connect at a single point (e.g. a steel beam to a column). This can result in two equally problematic solutions:
	- A monolithic function directly coupled to all the objects and how they might interact, which becomes a serious code maintenance problem as the complexity of different cobinations increases.
	- Mutual knowledge of other objects built into objects, resulting in a similar problem as above but compounded by the complexity of coordinating the distributed knowledge into a single solution.
- Tool interactions (similar to object interations above) where the action of one tool on objects may require corollary actions by other tools, e.g. a tool that changes the height of a storey in a building (potentially changing other storey positions and shape/size of any number of other components linked with the building shell or structure). Similar monolithic or distributed solutions can be employed (as described above) with similar problems.

## Overview <a name="over"></a>

Interactions between decoupled objects are supported through a publish/subscribe model, roughly analogous to the distribution of written contributions to readers through some publishing process:
- An author produces articles, but can't divide their time maintaining customer lists and posting articles to them. This is delegated to a publisher who:
	- Receives articles from authors
	- Maintains lists of subscribers who have registered an interest in receiving specific types of articles and publishes articles accordingly
	- Facilitates a postbox for subscribers to send messages back to the author
- The author and subscribers are completely independent - a change in circumstances to one has no affect on the other.

This is reflected in the structure of this module:

- `Event`: Equivalent to a published 'article', i.e. information generated at some source that may be of interest to any number of other objects. An `Event` is derived from:
	- `NameID`: An identity that is key to delivering to the correct `Subscribers`. The ID can be simply text, but can also include a guid if there is a risk of coincidental overlap.
	- `SettingList`: A container for data attaching data that `Subscribers` might need. Refer to the documentation for `SettingList` in the Settings module for more detail.
- `Subscriber`: An object interested in receiving some kind of `Event`
- `Publisher`: Collects `Subscribers` and requests a subscription list of the type of `Events` they are interested in. Incoming `Events` are published to appropriate `Subscribers` in order of priority.
- `PostBox`: The author of an `Event` can add a link to a `PostBox` if they want responses from `Subscribers`
- `Message`: A `Message` can be sent by a `Subscriber` to the `Event` author, which is lodged in the linked `PostBox`. The `Message` will be discarded if the `Event` author has made no provision to receive it.
- `Participant`: Both `Publishers` and `Subscribers` share some common traits expressed in the base `Participant` class:
	- Derived from `NameID`,` so both can be uniquely identified (both to avoid duplicating subscription patterns and to support logging of event-handling processes). Note that a guid must be used in this case for unique identification - the name is optional but helpful for logging.
	- Supporting modular loading/installation patterns. Both might be need to support a structured pattern when loaded and unloaded, e.g. in the case of a DLL-based plugin. A series of functions can be implemented to facilitate actions at keys points:
		- `audit()`: Called when first loaded, this function should check for any circumstances that would prevent participation in event-handling, e.g. hardware limitations, limited licensing etc. If this function returns false, the `Publisher` or `Subscriber` will not be loaded or activated.
		- `attach()`: Called after a successful audit. This function is an opportunity for the module to attach itself to the context it's required to function in, e.g. a tool participating in a menu or tool-bar might register its involvement and/or specify required resources. Again, returning false prevents the module from loading or participating in event-handling.
		- `start()`: Called after successful attachment. The module is ready to start operation and should allocate any necessary resources etc required to function. 
		- `stop()`: Called when participation in event-handling should stop. Any resources or other attachments should be released at this point.

## Detail <a name="implement"></a>

This solution is fast and highly scalable, but it's important to understand some of the detail.

On receiving an Event, the Publisher:
- Filters current `Subscribers` for any that specified an interest
- Sorts `Subscribers` on the basis of their stated priority
- The `Event` is sent to each `Subscriber` in turn (serial queue)
- If the call to `Subscriber::receive` returns *true*, publication of the `Event` will cease (used in cases where only one solution is applicable - once found, no others should be attempted)

There are many options for processing `Events` serially or concurrently:
- An `Event` can be sent through a `Publisher` from multiple threads concurrently. Each `Event` is published to relevant `Subscribers` on a serial queue, concurrently with other published events
- If an `Event` author is expecting a `Message` response through a `PostBox` attached to the `Event`, it will have to wait for `App::publish` to return before it can process the response(s).
- If waiting for a response is not appropriate, an `Event` author can `Publish` the `Event` on a separate, concurrent thread and rely on one or more `Subscribers` to publish reciprocal `Events` on completion (allowing the original author to activate as/when a response is available)
- If `Subscribers` are not replying through a `PostBox`, they can launch a process on a separate thread and return immediately to the `Publisher` (enabling concurrent processing of `Events`).

## Example <a name="example"></a>

Refer to the `EventTests` suite, specifically `testEventHandling`, which creates a simple calculator tool and simulates some user actions. The following sections break down the different components and discuss how `Events` make them work together.

### App <a name="app"></a>

A basic app context is defined in CalcApp.h:
```Cpp
class CalcApp : public App {};
```
The `App` class is defined as a `Publisher`, which in this context enables it to be the broker between:
- UI components
- A `Calculator` tool
- Calculation implementations

This arrangement ensures:
- The UI components know nothing of their purpose or connection to any tool;
- The calculation functions know nothing of a calculator or UI - they just look for expected inputs, calculate a result and send it as a message;
- The tool does not know anything about its UI, what buttons/calculations are available, or how calculations are performed (it's only job is to manage a value stack and calculate a current display value).

While this is a relatively modest context, these principles can break very complex problems into small, well-defined parts. This has 2 benefits:
1. The development process benefits from the decoupled components - the UI, tools and functions can all be developed as independently projects and gradually plugged in as they become available.
2. Maintenance is rudimentary because changes have no impact beyond a narrow, local context, e.g. fixing/improving a calculation function can have no repercussions on the UI or the calculator tool.

### Button <a name="button"></a>

The test suite works with a simulation rather than a full app with a real UI. A Button class is defined as a shell for testing the handling of events through UI components:

```Cpp
class Button {
public:
	/*!
		Constructor
		@param type The event type emitted when pressed
		@param action The action setting attached to an event when pressed
	*/
	Button(NameID type, String action) : m_type{type}, m_action{action} {}
	
		///Trigger a button press
	void operator()() const { calcApp->publish(Event{m_type, {{StringValue{m_action}, actionID}}}); }

private:
	NameID m_type;
	String m_action;
};
```
Clicking the button simply publishes the assigned `Event` through the `CalcApp`, i.e.:
```Cpp
	calcApp->publish(Event{m_type, {{StringValue{m_action}, actionID}}});
```

### Calculation <a name="calc"></a>

This class is the base for any tool providing calculation services to the app:
```Cpp
class Calculation : public Subscriber {
public:
		///Get the stack depth consumed by the calculation
	virtual int32_t depth() const { return 2; }
		///Receive a subscribed event - return true if event request is handled by this tool
	bool receive(const Event& event) override {
		if (auto stack = event.findValue(parameterID); (stack != nullptr) && (stack->size() >= depth())) {
				//Attempt to perform the calculation
			if (auto message = perform(*stack); message) {
					//Add the number of consumed figures to the return message
				message->emplace_back(ValueSetting{Int32Value{depth()}, usedID});
					//Send the result to the author
				event.tellAuthor(std::move(*message));
				return true;
			}
		}
		return false;
	}
		///Perform the calculation and return the result as a message (nullopt on failure)
	virtual std::optional<Message> perform(const ValueSetting& stack) { return std::nullopt; }
};
```
Any calculation is:
- Waiting for a specific event type
- Expecting a stack of numbers with a minimum number of values to perform a calculation
- Coded to perform a specific calculation, e.g. multiplication, addition etc.

The key function is `receive` - at this point, any calculation will need to check that a number stack of sufficient depth has been received with the `Event` before performing the calculation.

`Multiply`, for example, only has to subclass Calculation and implement the `subscription` and `perform` functions like so:
```Cpp
class Multiply : public Calculation {
public:
		//Get the event subscription list
	Subscription subscription() const override  { return { multiplyID }; }
		///Perform the calculation and return the result as a message (nullopt on failure)
	std::optional<Message> perform(const ValueSetting& stack) override {
		return Message{{DoubleValue{stack.doubleVal(stack.size() - 2) * stack.doubleVal(stack.size() - 1)}, resultID}};
	}
};
```
Note that the `subscription` function advises a `Publisher` that this tool wants to receive requests for a `Multiply` calculation service. Each tool will do likewise for its range of services.

The calculation tools make their services available to the app (or any other `Publisher`) like so:
```Cpp
	calcApp->add<Multiply>();
```

Those tool services are now available for any service requests through the app.

### Calculator <a name="calculator"></a>

The Calulator's role is:
- Manage a string for the current display
- Manage a stack of user-entered numbers
- Respond to button press events and forward some of them on to calculations
> NB: Some of the class members are omitted to focus on the Event-based functions - refer to `EventTests` for the full class
```Cpp
class CalculatorRPN : public Subscriber {
public:
		///Subscriptions required - in this case user input of either digits or calculator operations
	Subscription subscription() const override { return { digitID, operationID }; };
		///Receive a subscribed event
	bool receive(const Event& event) override {
		if (event == digitID)
			enterDigit(event);
		else
			performOperation(event);
		return true;
	}
};
```
The `subscription()` function lets the `Publisher` know that the calculator wants `Events` for a digit or function operator. And `receive` handles these events accordingly - handling of an operator is of particular note:
```Cpp
void CalculatorRPN::performOperation(const Event& event) {
		//Get the operator action from the event
	if (auto action = event.findValue(actionID); (action != nullptr) && !action->empty()) {
		PostBox response;
			//Publish an event seeking a calculation
		if (calcApp->publish(Event{action->stringVal(), { ValueSetting{m_valueStack, parameterID} }, &response}) && (response.size() == 1)) {
				//Seek the calculation result and number of stack values used
			if (auto result = response.front()->findValue(resultID), used = response.front()->findValue(usedID);
						(result != nullptr) && (used != nullptr) && !result->empty() && result->isGood()) {
					//Pop used values off the stack
				for (auto i = used->int32Val(); i--; )
					m_valueStack.pop_back();
					//Push the result onto the stack and refresh the display accordingly
				m_valueStack.push_back(result->doubleVal());
				refreshDisplay();
				return;
			}
		}
			//An error has occurred if we reach this point - clear the value stack and display an error code
		m_valueStack.clear();
		m_valueStack.push_back(0);
		m_display = "E";	//Display an error
	}
}
```
It expects a function action to be attached to the event as a named setting (which is provided by a button). This action is then published as the ID of an event requesting a calculation using the attached value stack. A `PostBox` is also attached to the `Event` because the result is expected to be returned as a `Message` (from the `Calculation`).

The response `Message` is searched for the result and the number of values used in the calculation. The used values are popped from the stack and the result pushed.

Note that this code has no mention of a UI or the tools that make calculations. These could be extended to any level of complexity without changing this code at all - this is essentially the goal of the Events module.
