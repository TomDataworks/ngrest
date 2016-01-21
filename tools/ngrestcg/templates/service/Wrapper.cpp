// This file generated by ngrest_codegen
// For more information please visit: https://github.com/loentar/ngrest
// DO NOT EDIT. ANY CHANGES WILL BE LOST

#include <ngrest/utils/Log.h>
#include <ngrest/utils/fromcstring.h>
#include <ngrest/utils/tostring.h>
#include <ngrest/utils/Exception.h>
#include <ngrest/common/ObjectModel.h>
#include <ngrest/common/ObjectModelUtils.h>
##ifneq($(interface.services.$count),0)
#include <ngrest/common/Message.h>
#include <ngrest/common/HttpMethod.h>
#include <ngrest/common/Service.h>
#include <ngrest/engine/ServiceDescription.h>
##endif
#include "$(interface.filePath)$(interface.name)Wrapper.h"
\
##var lastNs
##var lastNsEnd
\
##include <common/enums.cpp>
##include <common/typedefs.cpp>
##include <common/structs.cpp>

##ifneq($(interface.services.$count),0)
##foreach $(interface.services)

##include <common/nsopt.cpp>

$(service.name)Wrapper::$(service.name)Wrapper():
    service(new $(service.name)())
{
}

$(service.name)Wrapper::~$(service.name)Wrapper()
{
    delete service;
}


::ngrest::Service* $(service.name)Wrapper::getServiceImpl()
{
    return service;
}

void $(service.name)Wrapper::invoke(const ::ngrest::OperationDescription* operation, ::ngrest::MessageContext* context)
{
    \
##foreach $(service.operations)
if (operation->name == "$(operation.name)") {   //  **************** $(operation.name) *****************

        /// $(operation.return) $(operation.name)($(operation.params));

##ifeq($(operation.isAsynch),false)     //////////// synchronous ///////////////
\
##include <common/serviceRequest.cpp>
\
######### invoke the service synchronously ###########
##ifneq($(operation.return),void)
##ifeq($(operation.return.type),struct||typedef||template||string)
        const $(operation.return)& result = \
##else
        $(operation.return) result = \
##endif // const ref
##else  // void
        \
##endif
\
service->$(operation.name)(\
##var splitter
##foreach $(operation.params)
##ifeq($($splitter),)
##var splitter 1
##else
, \
##endif
$(param.name)\
##endfor
);

##context $(operation.return)
##include <common/serviceResponse.cpp>
##endcontext
\
\
##else                                  //////////// asynchronous ///////////////
\
##include <common/serviceRequest.cpp>

        class Callback_$(operation.name): public Callback< $($callbackType) >
        {
        public:
            Callback_$(operation.name)(::ngrest::MessageContext* context_):
                context(context_)
            {
            }

            void success($($callbackType) result) override
            {
##foreach $(operation.params)
##ifeq($(param.dataType.name),Callback)
##indent +2
##context $(param.dataType.templateParams.templateParam1)
##include <common/serviceResponse.cpp>
##endcontext
##indent -2
##endif
##endfor
                // no need to "delete this" - it's in mempool
            }

            virtual void error(const ::ngrest::Exception& error)
            {
                context->callback->error(error);
                // no need to "delete this" - it's in mempool
            }

            ::ngrest::MessageContext* const context;
        };

        service->$(operation.name)(\
##var splitter
##foreach $(operation.params)
##ifeq($($splitter),)
##var splitter 1
##else
, \
##endif
##ifneq($(param.dataType.name),Callback)
$(param.name)\
##else
*context->pool.alloc<Callback_$(operation.name)>(context)\
##endif
##endfor
);

##endif
    } else \
##endfor
{
        NGREST_THROW_ASSERT("No operation " + operation->name + " found in service $(service.name)");
    }
}

const ::ngrest::ServiceDescription* $(service.name)Wrapper::getDescription()
{
    static ::ngrest::ServiceDescription description = {
        "$(.nsName.!dot)",
        "$(.options.*location)",
        {
##var isComma 0
##foreach $(.operations)
##ifeq($($isComma),0)
##var isComma 1
##else
,
##endif
            {
                "$(.name)",
##ifeq($(.options.*location)-$(.options.*method||"GET"),-GET)
##var loc
##ifneq($(operation.params.$count),0)
### generate location for get query
##var loc $(.name)?
##var isAmp 0
##foreach $(operation.params)
##ifeq($($isAmp),0)
##var isAmp 1
##else
##var loc $($loc)&
##endif
##var loc $($loc)$(param.name)={$(param.name)}
##endfor
##endif
                "$($loc)",
##else
                "$(.options.*location)",
##endif
                static_cast<int>(::ngrest::HttpMethod::$(.options.*method||"GET")),
                "$(.options.*method||"GET")",
                $(.isAsynch||"false")
            }\
##endfor

        }
    };

    return &description;
}


##endfor
$($lastNsEnd)
##endif // ##ifneq($(interface.services.$count),0)