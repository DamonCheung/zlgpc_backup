#include <linux/slab.h>

/*创建slab缓存*/
static kmem_cache_t *xxx_cachep;
xxx_cachep = kmem_cache_create("xxx", sizeof(struct xxx), 0, SLAB_HWMCACHE_ALIGN | SLAB_PANIC, NULL, NULL);

/*分配slab缓存*/
struct xxx *ctx;
ctx = kmem_cache_alloc(xxx_cachep,GFP_KERNEL);
//......
/*使用缓存*/
//......
/*释放缓存*/
kmem_cache_free(xxx_cachep,ctx);
/*收回slab缓存*/
kmem_cache_destroy(xxx_cachep);




