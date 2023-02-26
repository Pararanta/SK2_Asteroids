#include <messages.h>

Request ntoh_req(Request request)
{
    request.type = ntohs(request.type);
    if(request.type == REQUEST_ENTITY)
        request.index = ntohs(request.index);
    
    return request;
}

Request hton_req(Request request)
{
    request.type = htons(request.type);
    if(request.type == REQUEST_ENTITY)
        request.index = htons(request.index);
    
    return request;
}

Response ntoh_res(Response response)
{
    response.index = ntohs(response.index);
    response.entity.type = ntohs(response.entity.type);
    
    return response;
}

Response hton_res(Response response)
{
    response.index = htons(response.index);
    response.entity.type = htons(response.entity.type);
    
    return response;
}