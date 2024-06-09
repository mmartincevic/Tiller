#include <iostream>
#include <exception>
#include <string>

namespace tiller
{
    class TillerException : public std::exception {
        public:
            explicit TillerException(const std::string& message)
                : message_(message) {}

            virtual const char* what() const noexcept override {
                return message_.c_str();
            }

        private:
            std::string message_;
    };
};
