#pragma once


#include "entityx\System.h"
#include "EventLibrary.h"

namespace Raven {

	class TimerSystem : public ex::System<TimerSystem>, public ex::Receiver<TimerEvent> {
		
		explicit TimerSystem() {
			eventToggle = true;
		}

		void configure(ex::EventManager &event_manager) {
			event_manager.subscribe<TimerEvent>(*this);
		}

		void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;

		template <typename T>
		void receiveEvent(const T &timerEvent) {
			eventToggle = eventToggle ? response(timerEvent) && false : true;
		}

		
		void receive(const TimerEvent &timerEvent) {
			receiveEvent(timerEvent);
		}
		

		bool response(const TimerEvent &timerEvent) {
			
			std::map<std::string, Timer>::iterator it = timerEvent.timeTable->timerMap.find(timerEvent.timerName);

			if (it != timerEvent.timeTable->timerMap.end()) {
				if (timerEvent.timerOperation == cmn::ETimerOperation::ADD) {
					cerr << "Error: Attempt to add timer when timer was found in timerMap." << endl;
					return true;
				}
			}
			else {
				if (timerEvent.timerOperation != cmn::ETimerOperation::ADD) {
					cerr << "Error: Attempt to perform operation on non-existent timer" << endl;
					return true;
				}
			}
			
			Timer timer = timerEvent.timeTable->timerMap[timerEvent.timerName];

			switch (timerEvent.timerOperation) {
			case cmn::ETimerOperation::NO_OPERATION:
				cerr << "Error: Timer event emitted with no operation." << endl;
				break;
			case cmn::ETimerOperation::PLAY:
				timer.play();
				break;
			case cmn::ETimerOperation::PAUSE:
				timer.pause();
				break;
			case cmn::ETimerOperation::SCAN:
				timer.scan(timerEvent.scanTime);
				break;
			case cmn::ETimerOperation::RESET:
				timer.reset();
				break;
			case cmn::ETimerOperation::ADD:
				timerEvent.timeTable->timerMap[timerEvent.timerName] = timer;
				break;
			case cmn::ETimerOperation::REMOVE:
				timerEvent.timeTable->timerMap.erase(timerEvent.timerName);
				break;
			}

			return true;
		}
		

	private:
		bool eventToggle;
	};
}
