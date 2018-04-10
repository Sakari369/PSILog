#include <memory>

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
	//std::unique_ptr<Logger> logger = std::make_unique<Logger>();
	Logger logger;

	logger.set_log_filter(Logger::INFO | Logger::WARN | Logger::ERR);

	logger.add_output(move(make_unique<LoggerConsoleOutput>()));
	logger.add_output(move(make_unique<LoggerFileOutput>("/tmp/log_test.txt")));

	logger(Logger::INFO) << "All systems initialized" << std::endl;
	logger(Logger::WARN) << "WARNING: Phasers damaged" << std::endl;
	logger(Logger::ERR)  << "ERROR: Failed to boot phasers" << std::endl;

	// Test out logging user types
	WarpDrive drive;
	drive.set_status(WarpDrive::ACTIVE);
	logger(Logger::INFO) << drive << std::endl;

	return 0;
}
