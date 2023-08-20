
# 定义变量SRC_LIST,代表当前文件加下所有源文件
aux_source_directory(. SRC_LIST)
aux_source_directory(./MysqlConnectionPool DB_LIST)
aux_source_directory(./model MODEL_LIST)
aux_source_directory(./redis REDIS_LIST)

# 指定生成可执行文件
add_executable(chatserver ${SRC_LIST} ${DB_LIST} ${MODEL_LIST} ${REDIS_LIST})

# 可执行文件需要链接的库 
target_link_libraries(chatserver muduo_net muduo_base libmysqlclient.so hiredis pthread)
