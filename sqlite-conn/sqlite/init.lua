local driver = require('sqlite.driver')

local conn_mt

local function connect(opts)
    opts = opts or {}

    local s, c = driver.connect(opts.db)
    if s == nil then
        if opts.raise then
            error(c)
        end
        return nil, c
    end

    return setmetatable({
        driver = c,

        -- connection variables
        db          = opts.db,

        -- private variables
        queue       = {},
        processing  = false,

        -- throw exception if error
        raise       = opts.raise
    }, conn_mt)
end

local function close(self)
    return self.driver:close()
end

local function execute(self, sql)
    local tuples = self.driver.execute(self.driver, sql)
    return tuples
end

conn_mt = {
    __index = {
        close = close;
        execute = execute;
    }
}

return {
    connect = connect;
}