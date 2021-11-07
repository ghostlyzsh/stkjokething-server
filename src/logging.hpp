#ifndef LOGGING_H_INCLUDED
#define LOGGING_H_INCLUDED

namespace logging {
    // debug, info, warn, err, fatal
    void debug(std::string proc, std::string message) {
	std::cout << "[" << proc << "/DEBUG] " << message << std::endl;
    }

    void log(std::string proc, std::string message) {
	std::cout << "[" << proc << "/INFO] " << message << std::endl;
    }

    void warn(std::string proc, std::string message) {
	std::cout << "[" << proc << "/WARN] " << message << std::endl;
    }

    void error(std::string proc, std::string message) {
	std::cout << "[" << proc << "/ERROR] " << message << std::endl;
    }

    void fatal(std::string proc, std::string message) {
	std::cout << "[" << proc << "/FATAL] " << message << std::endl;
    }
}

#endif
