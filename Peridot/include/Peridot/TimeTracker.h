#pragma once

namespace Peridot {
	class TimeTracker {
	public:
		TimeTracker();
		float Delta();
	private:
		float mTimePoint = 0.0f;
	};
}