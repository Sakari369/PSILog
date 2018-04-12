#include <memory>
#include <thread>
#include <chrono>
#include <future>

#include "logger.h"

// A custom user type, for testing out logging user types
class WarpDrive {
public:
	enum DriveStatus {
		DISABLED = 0,
		ACTIVE = 1
	};

	WarpDrive() = default;
	~WarpDrive() = default;

	int get_status() const { return _status; }
	void set_status(int status) { _status = status; }

	std::string get_model_name() const { return _model_name; }
	void set_model_name(const std::string &model_name) { _model_name = model_name; }

	friend std::ostream& operator << (std::ostream& os, const WarpDrive& drive) {
		os << "WarpDrive model " << drive.get_model_name()
		   << " status = " << drive.get_status();

		return os;
	}

private:
	std::string _model_name = "AGDrive 9000";
	int _status = DISABLED;
};

int main(int argc, char *argv[]) {
	Logger logger;

	// Set filtering level, binary arithmetic
	//logger.set_log_filter(Logger::INFO | Logger::WARN | Logger::ERR);
	logger.set_log_filter(Logger::ALL);

	// TODO: would be it more clear to do the move insde add_output ?
	logger.add_output(move(make_unique<LoggerConsoleOutput>()));
	logger.add_output(move(make_unique<LoggerFileOutput>("/tmp/log_test.txt")));

	// Example idea of how to log into a network service
	//logger.add_output(move(make_unique<LoggerXhrOutput>("https://127.0.0.1:3000/log", auth_info);
	logger(Logger::INFO) << "All systems initialized" << std::endl;
	logger(Logger::WARN) << "WARNING: Phasers damaged" << std::endl;
	logger(Logger::ERR)  << "ERROR: Failed to boot phasers" << std::endl;

	// Test out logging user types
	WarpDrive drive;
	drive.set_status(WarpDrive::ACTIVE);
	logger(Logger::INFO) << drive << std::endl;

	// lambda function for testing threading
	auto t_func1 = [&logger] (int level) {
		int delay = 500 + level*100;
		logger(Logger::FREQ)  << "Stabilizing phaser " << level << ", time remaining = " << delay << " ms";
		logger.flush();
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		logger(Logger::FREQ)  << "Phaser " << level << " stabilized." << std::endl;
	};

	auto t_func2 = [&logger] (int level) {
		logger(Logger::FREQ)  << "Phaser " << level*10 << " ready, stabilizing immediately" << std::endl;
	};

	// Test threading
	// How would we test launch these so that they are attempting to write at the same time ?
	// Would have to add messages to the logger at the same time
	for (int i=0; i<6; i++) {
		std::thread t1(t_func1, i);
		std::thread t2(t_func2, i*2);
		//std::thread t3(t_func2, i*3);
		t1.join();
		t2.join();
		//t3.join();
	}

	logger(Logger::INFO)  << "All phasers stabilized" << std::endl;

	logger(Logger::INFO)  << "Shutting down all systems ..." << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	logger(Logger::INFO)  << "Shutdown complete" << std::endl;

	return 0;
}
