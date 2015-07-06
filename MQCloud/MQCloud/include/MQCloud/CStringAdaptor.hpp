#include <functional>
#include <MQCloud/MQCloud.h>

#ifndef CStringAdaptorHpp
#define CStringAdaptorHpp
namespace MQCloud {
    struct CStringAdaptor {
        explicit CStringAdaptor(const CString *handler);

        const CString *GetData();

        std::string GetDataCopy();

        void        SetData(std::string &&_data);

        void        SetDataCopy(std::string _data);

        void        SetData(const std::string *_data, std::function<void()> OnDataRemoval);

        void        SetData(const CString *_data, void (*OnDataRemoval)());

        ~CStringAdaptor();

    private:
        CString *handler; // ReadOnly!
        std::function<void()> clearData;
    };
}
#endif // CStringAdaptorHpp

