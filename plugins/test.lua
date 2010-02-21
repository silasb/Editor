function printf(...) io.write(string.format(unpack(arg))) end

printf("silas %s\n", "rocks")

-- the c++ program will call this method
function my_function()
  print("hello world")
end

-- why even do this when we can have the c++ program exec a function in here
--register_callback("object", "slot_reload", my_function);
