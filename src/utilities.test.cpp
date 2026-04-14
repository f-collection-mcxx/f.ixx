// ReSharper disable CppExpressionWithoutSideEffects
import f;
import std;

int main() {
    // Test 1: equal_or_throw with matching value
    {
        try {
            f::equal_or_throw<int> eq{10};
            eq >> 10;
            std::println("✓ Test 1 passed: equal_or_throw with matching value");
        } catch (...) {
            std::println("✗ Test 1 failed: should not throw for matching value");
            return 1;
        }
    }

    // Test 2: equal_or_throw with non-matching value (void error type)
    {
        try {
            f::equal_or_throw<int> eq{10};
            eq >> 20;
            std::println("✗ Test 2 failed: should throw for non-matching value");
            return 1;
        } catch (const std::runtime_error& e) {
            std::println("✓ Test 2 passed: equal_or_throw throws exception: {}", e.what());
        }
    }

    // Test 3: equal_or_throw with custom error
    {
        try {
            f::equal_or_throw eq{5, std::invalid_argument{"custom error"}};
            eq >> 10;
            std::println("✗ Test 3 failed: should throw");
            return 1;
        } catch (const std::invalid_argument& e) {
            if (std::string{e.what()} == "custom error") {
                std::println("✓ Test 3 passed: equal_or_throw with custom error");
            } else {
                std::println("✗ Test 3 failed: wrong error message");
                return 1;
            }
        }
    }

    // Test 4: not_equal_or_throw with non-matching value
    {
        try {
            f::not_equal_or_throw<int> neq{10};
            neq >> 20;
            std::println("✓ Test 4 passed: not_equal_or_throw with non-matching value");
        } catch (...) {
            std::println("✗ Test 4 failed: should not throw");
            return 1;
        }
    }

    // Test 5: not_equal_or_throw with matching value (void error type)
    {
        try {
            f::not_equal_or_throw<int> neq{10};
            neq >> 10;
            std::println("✗ Test 5 failed: should throw for matching value");
            return 1;
        } catch (const std::runtime_error& e) {
            std::println("✓ Test 5 passed: not_equal_or_throw throws exception: {}", e.what());
        }
    }

    // Test 6: not_equal_or_throw with custom error
    {
        try {
            f::not_equal_or_throw neq{10, std::logic_error{"unexpected value"}};
            neq >> 10;
            std::println("✗ Test 6 failed: should throw");
            return 1;
        } catch (const std::logic_error& e) {
            if (std::string{e.what()} == "unexpected value") {
                std::println("✓ Test 6 passed: not_equal_or_throw with custom error");
            } else {
                std::println("✗ Test 6 failed: wrong error message");
                return 1;
            }
        }
    }

    // Test 7: nonnull with non-null pointer
    {
        try {
            int x = 42;
            int* ptr = &x;
            f::nonnull >> ptr;
            std::println("✓ Test 7 passed: nonnull with valid pointer");
        } catch (...) {
            std::println("✗ Test 7 failed: should not throw for non-null pointer");
            return 1;
        }
    }

    // Test 8: nonnull with null pointer
    {
        try {
            int* ptr = nullptr;
            f::nonnull >> ptr;
            std::println("✗ Test 8 failed: should throw for null pointer");
            return 1;
        } catch (const std::runtime_error& e) {
            std::println("✓ Test 8 passed: nonnull throws for null pointer: {}", e.what());
        }
    }

    // Test 9: Chaining equal_or_throw
    {
        try {
            f::equal_or_throw<int> eq1{10};
            f::equal_or_throw<int> eq2{20};
            (eq1 >> 10);
            (eq2 >> 20);
            std::println("✓ Test 9 passed: chaining equal_or_throw");
        } catch (...) {
            std::println("✗ Test 9 failed");
            return 1;
        }
    }

    std::println("\n✓ All utilities tests passed!");
    return 0;
}
