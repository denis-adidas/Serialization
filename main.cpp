#include <iostream>
#include <vector>

namespace Core {
    namespace util {

    }

    template <typename T>
    void encode(std::vector<int8_t>* buffer, int16_t* iterator, T value) {
         for (unsigned i = 0, j = 0; i < sizeof(T); i++) {
             (*buffer)[(*iterator)++] = (value >> ((sizeof(T) * 8) - 8 ) - ((i == 0) ? j : j += 8));
         }
    };

}

namespace ObjectModel {

    enum class Wrapper {
        PRIMITIVE = 1,
        ARRAY,
        STRING,
        OBJECT
    };
    enum class Type : int8_t {
        I8 = 1,
        I16,
        I32,
        I64,

        U8,
        U16,
        U32,
        U64,

        FLOAT,
        DOUBLE,

        BOOL
    };
    class Root {
    protected:
        std::string name;
        int16_t nameLength;
        int8_t wrapper;
        int32_t size;
    protected:
        Root();
    public:
        int32_t getSize();
        void setName(std::string);
        std::string getName();
        virtual void pack(std::vector<int8_t>*, int16_t);
    };
    class Primitive : public Root {
    private:
        int8_t type;
        std::vector<int8_t>* data;
    private:
        Primitive();
    public:
        static Primitive* createI32(std::string name, Type type, int32_t value);
        static Primitive* createI64(std::string name, Type type, int64_t value);
    };
    class Array : public Root {

    };
    class Object : public Root {

    };
    Root::Root()
    :
    name("unknown"),
    wrapper(0),
    nameLength(0),
    size(sizeof nameLength + sizeof wrapper + sizeof size) {}

    void Root::setName(std::string name) {
        this->name = name;
        nameLength = (int16_t)name.length();
        size += nameLength;
    }

    std::string Root::getName() {
        return name;
    }

    int32_t Root::getSize() {
        return size;
    }
    Primitive* Primitive::createI32(std::string name, ObjectModel::Type type, int32_t value) {
        Primitive* p = new Primitive();
        p->setName(name);
        p->wrapper = static_cast<int8_t>(Wrapper::PRIMITIVE);
        p->type = static_cast<int8_t>(type);
        p->data = new std::vector<int8_t>(sizeof data);
        int16_t iterator = 0;
        Core::encode(p->data, &iterator, value);


        return p;
    }
}



namespace EventSystem {
    class Event;


    class System {
    private:
        friend class Event;
        std::string name;
        int32_t description;
        int16_t index;
        bool active;
        std::vector<Event*> events;
    public:
        System(std::string);
        ~System();
        void addEvent(Event*);
        Event* getEvent();
        bool isActive();
        void serialize();
    };
    class Event {
    public:
        enum DeviceType : int8_t {
            KEYBOARD = 1,
            MOUSE,
            TOUCHPAD,
            JOYSTICK
        };
        DeviceType dType;
        System* system = nullptr;
    public:
        Event(DeviceType);
        DeviceType getType();
        friend std::ostream& operator<< (std::ostream& stream, const DeviceType dType) {
            std::string result;
#define PRINT(a) result = #a;
            switch (dType) {
                case KEYBOARD: PRINT(KEYBOARD); break;
                case MOUSE: PRINT(KEYBOARD); break;
                case TOUCHPAD: PRINT(KEYBOARD); break;
                case JOYSTICK: PRINT(KEYBOARD); break;
            }
            return stream << result;
        }
        void bind(System*, Event*);
    };
    class KeyboardEvent : public Event {
    private:
        int16_t keyCode;
        bool pressed;
        bool released;
    public:
        KeyboardEvent(int16_t, bool, bool);
    };

    System::System(std::string name) :
        name(name), description(123), index(1), active(true) {}

        System::~System() {
        //TODO ::
    }
    void System::addEvent(EventSystem::Event * e) {
        e->bind(this, e);
    }
    Event* System::getEvent() {
        return events.front();
    }
    bool System::isActive() {
        if (!system)
            return false;
        return true;
    }

    void System::serialize() {

        
    }

    Event::Event(DeviceType dType) {
        this->dType = dType;
    }
    void Event::bind(System* system, Event* e) {
        this->system = system;
        this->system->events.push_back(e);
    }
    Event::DeviceType Event::getType() {
        return this->dType;
    }
    KeyboardEvent::KeyboardEvent(int16_t keyCode, bool pressed, bool realesed)
    :
    Event(Event::KEYBOARD),
    keyCode(keyCode),
    pressed(pressed),
    released(realesed) {};

}



using namespace EventSystem;

int main() {

    int32_t foo = 5;

#if 0
    System Foo("Foo");
    Event* e = new KeyboardEvent('a', true, false);
    Foo.addEvent(e);

    KeyboardEvent* kb = static_cast<KeyboardEvent*>(Foo.getEvent());

    Foo.serialize();
#endif
    return 0;
}
