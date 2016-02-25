1,src 
	RedLion\2.6.35.11\fs\seq_file.c

2,summary
	a,动态构造虚拟文件,向用户空间返回数据	
	b,处理
		vfs read
			1),用户空间数据size的需求量,po等标准vfs read参数
			2),处理eof
		seq_xxx
			1),从内核动态构造数据,需要知道类'eof'即内核已经构造完所有的数据	
	
	c,平衡
		vfs read的需求量及内核动态构造的数据多少.
	
3,struct
	struct seq_file {
		char *buf;			//为虚拟文件的存储空间,
		size_t size;		//虚拟文件空间的总大小
		size_t from;		//一次vfs read后再次vfs read时,如果count !=0,则from即时buf中可以继续开始读取的位置  <--------- 
		size_t count;		//虚拟文件中可以供vfs read的数据量                                                            \
		loff_t index;		//seq_next时 pos,                                                                             |
		loff_t read_pos;	//类似于vfs pos,多用于校验                                                                    |
		u64 version;                                                                                                      |
		struct mutex lock;                                                                                                |
		const struct seq_operations *op;                                                                                  |
		void *private;                                                                                                    |
	};                                                                                                                    |
		                                                                                                                  |
3,seq_read                                                                                                                |
	一般流程:                                                                                                             |
		void* p,loff_t pos;                                                                                               |
		p=m->op->start(m,&pos)                                                                                            |
			ret=m->op->show(m,p)                                                                                          |
			p=m->op->next(m,p,&pos)                                                                                       |
		m->op->stop(m, p)                                                                                                 |
	  p:在seq操作过程中需要处理的数据                                                                                     |
	pos:filp中的offset                                                                                                    |
    ref:RedLion\2.6.35.11\fs\proc\base.c,mounts_op                                                                        |
	seq_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)                                              |
	{                                                                                                                     |
		.....                                                                                                             |
		/* if not empty - flush it first */                                                                               |
		if (m->count) {                                                                                                   |
			n = min(m->count, size);                                                                                      |
			err = copy_to_user(buf, m->buf + m->from, n);<---------------------------------------------------------------/
		}
        .....
		while (1) {
			....
			if (unlikely(!m->count)) {			//即如果,start中没有show的数据,则next.直到找到一个可以show的数据
				p = m->op->next(m, p, &pos);
				m->index = pos;
				continue;
			}
			....	
		}
		Fill:									//通常需要处理的地方
			/* they want more? let's try to get some more */
			while (m->count < size) {
			....
			}
		err = copy_to_user(buf, m->buf, n);		//copy to user space
		....
	}

