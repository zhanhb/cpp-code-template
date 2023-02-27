#include <stdexcept>

int convert_to_seconds(int value, char unit) {
    switch (unit) {
        case 'w':
        case 'W':
            value *= 7;
            [[fallthrough]];
        case 'd':
        case 'D':
            value *= 24;
            [[fallthrough]];
        case 'h': // NOLINT(bugprone-branch-clone)
        case 'H':
            value *= 60;
            [[fallthrough]];
        case 'm':
        case 'M':
            value *= 60;
            [[fallthrough]];
        case 's':
        case 'S':
            break;
        default:
            throw std::invalid_argument("");
    }
    return value;
}
