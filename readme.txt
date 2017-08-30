 Description:
   * 2016/11/23
 * Purpose: notify the apppication the latest information disk write .
 * Usage, application create netlink then send a message to driver to tell "pid" 
               for recv notification
 * ioctr(100) to start. 
 * the message information is something like this.
 * Message format: ****[uint64 sector_offset,uint32 sector_count,char[SDANAME]]****
 * generally SDANAME should be less than 5, 
 * So the recv buffer larger than 20 should be enough

 * Deployment: "sudo insmod diskless.ko"

Usage:
1. Install driver.
2. Run application
3. Input 100 to start
4. You will find the disk change. output.
