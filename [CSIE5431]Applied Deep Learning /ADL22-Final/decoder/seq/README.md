#### Data IO

 - input: user(+course) embedding, label(seq of course tokens)  
    shape: (batch_size, length, embed_dim)   

 - output: seq of course tokens  
    shape: (batch_size, length, total_course_num)
    (target: right-shifted label)  