#include <messages.h>

Request ntoh_req(Request request)
{
    request.type = ntohs(request.type);
    if(request.type == REQUEST_ENTITY)
        request.entity = ntohs(request.entity);
    
    return request;
}

Request hton_req(Request request)
{
    request.type = htons(request.type);
    if(request.type == REQUEST_ENTITY)
        request.entity = htons(request.entity);
    
    return request;
}

Response ntoh_res(Response response)
{
    response.entity = ntohs(response.entity);
    response.type = ntohs(response.type);
    
    return response;
}

Response hton_res(Response response)
{
    response.entity = htons(response.entity);
    response.type = htons(response.type);
    
    return response;
}