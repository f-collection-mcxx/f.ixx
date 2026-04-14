import f;
import std;

class PolymorphicBase {
public:
    virtual ~PolymorphicBase() = default;
    virtual int value() const { return 0; }
};

class Derived : public PolymorphicBase {
public:
    int v;
    Derived(int val) : v(val) {}
    int value() const override { return v; }
};

int main() {
    std::pmr::memory_resource* resource = std::pmr::new_delete_resource();

    // Test 1: make_unique for single object
    {
        auto ptr = f::make_unique<int>(resource, 42);
        if (*ptr == 42) {
            std::println("✓ Test 1 passed: make_unique for int");
        } else {
            std::println("✗ Test 1 failed");
            return 1;
        }
    }

    // Test 2: make_unique for custom type
    {
        auto ptr = f::make_unique<Derived>(resource, 100);
        if (ptr->value() == 100) {
            std::println("✓ Test 2 passed: make_unique for custom type");
        } else {
            std::println("✗ Test 2 failed");
            return 1;
        }
    }

    // Test 3: make_unique_array
    {
        auto ptr = f::make_unique_array<int>(10, resource, 0);
        bool all_zero = true;
        for (int i = 0; i < 10; ++i) {
            if (ptr[i] != 0) {
                all_zero = false;
                break;
            }
        }
        if (all_zero) {
            std::println("✓ Test 3 passed: make_unique_array for int array");
        } else {
            std::println("✗ Test 3 failed");
            return 1;
        }
    }

    // Test 4: unique_upcast
    {
        auto derived_ptr = f::make_unique<Derived>(resource, 200);
        auto base_ptr = f::unique_upcast<PolymorphicBase>(std::move(derived_ptr));
        if (base_ptr->value() == 200) {
            std::println("✓ Test 4 passed: unique_upcast works");
        } else {
            std::println("✗ Test 4 failed");
            return 1;
        }
    }

    // Test 5: unique_downcast success
    {
        auto derived_ptr = f::make_unique<Derived>(resource, 300);
        auto base_ptr = f::unique_upcast<PolymorphicBase>(std::move(derived_ptr));
        auto downcasted = f::unique_downcast<Derived>(base_ptr);
        if (downcasted.has_value() && downcasted.value()->value() == 300) {
            std::println("✓ Test 5 passed: unique_downcast success");
        } else {
            std::println("✗ Test 5 failed");
            return 1;
        }
    }

    // Test 6: deleter for single object
    {
        f::deleter<int> del{resource};
        auto ptr = static_cast<int*>(resource->allocate(sizeof(int), alignof(int)));
        *ptr = 42;
        del(ptr);
        std::println("✓ Test 6 passed: deleter for single object");
    }

    // Test 7: deleter for array
    {
        f::deleter<int[]> del{5, resource};
        auto ptr = static_cast<int*>(resource->allocate(sizeof(int) * 5, alignof(int)));
        for (int i = 0; i < 5; ++i) {
            new (ptr + i) int{i};
        }
        del(ptr);
        std::println("✓ Test 7 passed: deleter for array");
    }

    // Test 8: dynamic_deleter
    {
        f::dynamic_deleter<PolymorphicBase> del{resource, sizeof(Derived), alignof(Derived)};
        auto ptr = static_cast<Derived*>(resource->allocate(sizeof(Derived), alignof(Derived)));
        new (ptr) Derived{400};
        del(ptr);
        std::println("✓ Test 8 passed: dynamic_deleter works");
    }

    // Test 9: sized_pool memory resource
    {
        f::sized_pool<1024> pool{resource};
        auto small_ptr = pool.allocate(100, alignof(double));
        pool.deallocate(small_ptr, 100, alignof(double));
        std::println("✓ Test 9 passed: sized_pool allocation");
    }

    // Test 10: sync_sized_pool memory resource
    {
        f::sync_sized_pool<1024> pool{resource};
        auto small_ptr = pool.allocate(200, alignof(double));
        pool.deallocate(small_ptr, 200, alignof(double));
        std::println("✓ Test 10 passed: sync_sized_pool allocation");
    }

    std::println("\n✓ All memory tests passed!");
    return 0;
}
