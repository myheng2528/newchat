const config_module = require('./config');
const Redis = require("ioredis");


const RedisCli = new Redis({
    host: config_module.redis_host,       // Redis服务器主机名
    port: config_module.redis_port,        // Redis服务器端口号
    password: config_module.redis_passwd, // Redis密码
  });
  
  
  /**
   * 监听错误信息
   */
  RedisCli.on("error", function (err) {
    console.log("RedisCli connect error");
    RedisCli.quit();
  });
  
  async function GetRedis(key) {

    try{
        const result = await RedisCli.get(key)
        if(result === null){
          console.log('result:','<'+result+'>', 'This key cannot be find...')
          return null
        }
        console.log('Result:','<'+result+'>','Get key success!...');
        return result
    }catch(error){
        console.log('GetRedis error is', error);
        return null
    }

  }

  async function QueryRedis(key) {
    try{
        const result = await RedisCli.exists(key)
        //  判断该值是否为空 如果为空返回null
        if (result === 0) {
          console.log('result:<','<'+result+'>','This key is null...');
          return null
        }
        console.log('Result:','<'+result+'>','With this value!...');
        return result
    }catch(error){
        console.log('QueryRedis error is', error);
        return null
    }

  }
  async function SetRedisExpire(key,value, exptime){
    try{
        // 设置键和值
        await RedisCli.set(key,value)
        // 设置过期时间（以秒为单位）
        await RedisCli.expire(key, exptime);
        return true;
    }catch(error){
        console.log('SetRedisExpire error is', error);
        return false;
    }
}

  (async () => {
    await SetRedisExpire("abc","123",600);
    let res = await GetRedis("abc");
    console.log(res);
  })();