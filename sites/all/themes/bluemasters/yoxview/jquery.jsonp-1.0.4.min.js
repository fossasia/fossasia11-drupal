// jquery.jsonp 1.0.4 (c) 2009 Julian Aubourg | MIT License
// http://code.google.com/p/jquery-jsonp/
(function($){var x=function(o){return o!==undefined&&o!==null;},H=$("head"),Z={},K={callback:"C",url:location.href};$.jsonp=function(d){d=$.extend({},K,d);if(x(d.beforeSend)){var t=0;d.abort=function(){t=1;};if(d.beforeSend(d,d)===false||t)return d;}
var _="",y="success",n="error",u=x(d.url)?d.url:_,p=x(d.data)?d.data:_,s=(typeof p)=="string",k=function(f){setTimeout(f,1);},S,P,i,j,U;p=s?p:$.param(p);x(d.callbackParameter)&&(p+=(p==_?_:"&")+escape(d.callbackParameter)+"=?");!d.cache&&!d.pageCache&&(p+=[(p==_?_:"&"),"_xx",(new Date()).getTime(),"=",1].join(_));S=u.split("?");if(p!=_){P=p.split("?");j=S.length-1;j&&(S[j]+="&"+P.shift());S=S.concat(P);}
i=S.length-2;i&&(S[i]+=d.callback+S.pop());U=S.join("?");if(d.pageCache&&x(Z[U])){k(function(){if(x(Z[U].e)){x(d.error)&&d.error(d,n);x(d.complete)&&d.complete(d,n);}else{var v=Z[U].s;x(d.dataFilter)&&(v=d.dataFilter(v));x(d.success)&&d.success(v,y);x(d.complete)&&d.complete(d,y);}});return d;}
var f=$("<iframe />");H.append(f);var F=f[0],W=F.contentWindow||F.contentDocument,D=W.document;if(!x(D)){D=W;W=D.getParentNode();}
var w,e=function(_,m){d.pageCache&&!x(m)&&(Z[U]={e:1});w();m=x(m)?m:n;x(d.error)&&d.error(d,m);x(d.complete)&&d.complete(d,m);},t=0,C=d.callback,E=C=="E"?"X":"E";D.open();W[C]=function(v){t=1;d.pageCache&&(Z[U]={s:v});k(function(){w();x(d.dataFilter)&&(v=d.dataFilter(v));x(d.success)&&d.success(v,y);x(d.complete)&&d.complete(d,y);});};W[E]=function(s){(!s||s=="complete")&&!t++&&k(e);};w=function(){W[E]=undefined;W[C]=undefined;try{delete W[E];}catch(_){}
try{delete W[C];}catch(_){}
D.open()
D.write(_);D.close();f.remove();}
k(function(){D.write(['<html><head><script src="',U,'" onload="',E,'()" onreadystatechange="',E,'(this.readyState)"></script></head><body onload="',E,'()"></body></html>'].join(_));D.close();});d.timeout>0&&setTimeout(function(){!t&&e(_,"timeout");},d.timeout);d.abort=w;return d;}
$.jsonp.setup=function(o){$.extend(K,o);};})(jQuery);