
#ifndef _MEMDEV_H_
#define _MEMDEV_H_

#ifndef MEMDEV_MAJOR
#define MEMDEV_MAJOR 279   /*Ô¤ÉèµÄmemµÄÖ÷Éè±¸ºÅ*/
#endif

#ifndef MEMDEV_NR_DEVS
#define MEMDEV_NR_DEVS 5    /*Éè±¸Êý*/
#endif

#ifndef MEMDEV_SIZE
#define MEMDEV_SIZE 4096
#endif

/*memÉè±¸ÃèÊö½á¹¹Ìå*/
struct mem_dev                                     
{                                                        
  char *data;                      
  unsigned long size;
  struct device *dev;
  dev_t devt;
};

#endif /* _MEMDEV_H_ */
