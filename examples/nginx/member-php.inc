# In the server block: AFTER dotfile denial, BEFORE generic script denial.
# Enable only after installing and testing the matching member FPM pool.
location ~ ^/(?:~|u/)member/(?<member_php_script>.+\.php)$ {
    # try_files changes $uri; retain the original public script URL for PHP.
    set $member_php_uri $uri;
    root /home/member/public_html;
    disable_symlinks on;
    try_files /$member_php_script =404;
    fastcgi_param QUERY_STRING $query_string;
    fastcgi_param REQUEST_METHOD $request_method;
    fastcgi_param CONTENT_TYPE $content_type;
    fastcgi_param CONTENT_LENGTH $content_length;
    fastcgi_param SCRIPT_FILENAME /home/member/public_html/$member_php_script;
    fastcgi_param SCRIPT_NAME $member_php_uri;
    fastcgi_param REQUEST_URI $request_uri;
    fastcgi_param DOCUMENT_URI $member_php_uri;
    fastcgi_param DOCUMENT_ROOT /home/member/public_html;
    fastcgi_param SERVER_PROTOCOL $server_protocol;
    fastcgi_param REQUEST_SCHEME $scheme;
    fastcgi_param HTTPS $https if_not_empty;
    fastcgi_param GATEWAY_INTERFACE CGI/1.1;
    fastcgi_param SERVER_SOFTWARE nginx;
    fastcgi_param REMOTE_ADDR $remote_addr;
    fastcgi_param REMOTE_PORT $remote_port;
    fastcgi_param REMOTE_USER $remote_user;
    fastcgi_param SERVER_ADDR $server_addr;
    fastcgi_param SERVER_PORT $server_port;
    fastcgi_param SERVER_NAME $server_name;
    fastcgi_param HTTP_PROXY "";
    fastcgi_pass unix:/run/php-fpm/tilde-member.sock;
}
