#############
# OpenStack #
#############

[composite:osapi_volume]
use = call:cinder.api:root_app_factory
/: apiversions
/v3: openstack_volume_api_v3

[composite:openstack_volume_api_v3]
use = call:cinder.api.middleware.auth:pipeline_factory
noauth = cors http_proxy_to_wsgi request_id faultwrap sizelimit osprofiler noauth apiv3
keystone = cors http_proxy_to_wsgi request_id faultwrap sizelimit osprofiler authtoken keystonecontext apiv3
keystone_nolimit = cors http_proxy_to_wsgi request_id faultwrap sizelimit osprofiler authtoken keystonecontext apiv3

[filter:request_id]
paste.filter_factory = oslo_middleware.request_id:RequestId.factory

[filter:http_proxy_to_wsgi]
paste.filter_factory = oslo_middleware.http_proxy_to_wsgi:HTTPProxyToWSGI.factory

[filter:cors]
paste.filter_factory = oslo_middleware.cors:filter_factory
oslo_config_project = cinder

[filter:faultwrap]
paste.filter_factory = cinder.api.middleware.fault:FaultWrapper.factory

[filter:osprofiler]
paste.filter_factory = osprofiler.web:WsgiMiddleware.factory

[filter:noauth]
paste.filter_factory = cinder.api.middleware.auth:NoAuthMiddleware.factory

[filter:sizelimit]
paste.filter_factory = oslo_middleware.sizelimit:RequestBodySizeLimiter.factory

[app:apiv3]
paste.app_factory = cinder.api.v3.router:APIRouter.factory

[pipeline:apiversions]
pipeline = cors http_proxy_to_wsgi faultwrap osvolumeversionapp

[app:osvolumeversionapp]
paste.app_factory = cinder.api.versions:Versions.factory

##########
# Shared #
##########

[filter:keystonecontext]
paste.filter_factory = cinder.api.middleware.auth:CinderKeystoneContext.factory

[filter:authtoken]
paste.filter_factory = keystonemiddleware.auth_token:filter_factory
