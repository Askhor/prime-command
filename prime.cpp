#include "tclap/CmdLine.h"

using namespace TCLAP;

enum LogLevel { Info = 0, Error = 1, Prime = 2 };
typedef unsigned long long integer;

template <typename T> void _log_objects(T t) { std::cout << t; }

template <typename T, typename... Args> void _log_objects(T t, Args... args) {
    std::cout << t << " ";
    _log_objects(args...);
}

template <typename... T> void log(LogLevel ll, bool verbose, T... values) {
    if (!verbose && (int)ll < 1) {
        return;
    }

    switch (ll) {
    case Info:
        std::cout << "[Info] \t";
        break;
    case Error:
        std::cout << "[Error] \t";
        break;
    case Prime:
        std::cout << "[Prime] \t";
        break;
    }

    _log_objects(values...);

    std::cout << std::endl;
}

bool is_prime(integer num, bool verbose = false) {
    log(Info, verbose, "Testing", num);
    if (num == 1) {
        log(Info, verbose, "One is not a prime number");
        return false;
    }
    if ((num & 1) == 0) {
        log(Info, verbose, "Skipping even number", num);
        return false;
    }

    for (integer i = 3; i * i < num; i += 2) {
        if (num % i == 0) {
            log(Info, verbose, num, "is divisible by", i);
            return false;
        }
    }

    return true;
}

int interface(bool verbose, bool test, integer number) {
    if (number < 1) {
        log(Error, verbose,
            "The number must be a positive integer. Instead:", number);
        return -1;
    }

    if (test) {
        bool result = is_prime(number, verbose);
        log(Prime, verbose, number, "is", result ? "" : "not",
            "a prime number");
        return result ? 0 : -1;
    } else {
        integer i = number;
        if ((i & 1) == 0) {
            is_prime(i, verbose);
            i++;
        }
        for (; !is_prime(i, verbose); i += 2)
            ;

        if (i == number)
            log(Prime, verbose, number, "is already prime");
        else
            log(Prime, verbose, i, "is the next prime number after", number);
        return 0;
    }
}

int main(int argc, char **argv) {
    try {
        CmdLine cmd("prime", ' ', "1.0");
        SwitchArg verbose("v", "verbose", "Enable step by step computations",
                          cmd);
        SwitchArg test("t", "test",
                       "Only test whether the given number is a prime. Also "
                       "outputs that information as return code. Success for "
                       "prime and fail for non-prime.",
                       cmd);
        UnlabeledValueArg<int> number(
            "number", "The number to test if prime/find next prime", true, 0,
            "int", cmd);
        cmd.parse(argc, argv);

        return interface(verbose.getValue(), test.getValue(),
                         number.getValue());
    } catch (ArgException &e) {
        std::cerr << "error:" << e.error() << " for arg " << e.argId()
                  << std::endl;
        return -1;
    }
}
