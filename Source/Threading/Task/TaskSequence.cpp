#include <Threading/Task/RecurentTask.hpp>
#include "TaskSequence.hpp"

namespace drak {
namespace thread {
namespace task {

void TaskSequence::operator()() {
	for each(ATask* pTask in m_taskList) {
		pTask->execute();
	}
	m_executed.store(true, std::memory_order_release);
}

void TaskSequence::execute() {
	for each(ATask* pTask in m_taskList) {
		pTask->execute();
	}
	m_executed.store(true, std::memory_order_release);
}

} // namespace task
} // namespace thread
} // namespace drak