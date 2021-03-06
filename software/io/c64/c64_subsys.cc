/*
 * c64_subsys.cc
 *
 *  Created on: Jul 16, 2015
 *      Author: Gideon
 */

#include "c64.h"
#include "c64_subsys.h"
#include <ctype.h>
#include "init_function.h"
#include "userinterface.h"
#include "u64.h"
#include "c1541.h"

#define C64_BOOTCRT_DOSYNC    0x014F
#define C64_BOOTCRT_RUNCODE   0x0172
#define C64_BOOTCRT_DRIVENUM  0x0173
#define C64_BOOTCRT_NAMELEN   0x0174
#define C64_BOOTCRT_NAME      0x0175
#define C64_BOOTCRT_HANDSHAKE 0x02
#define C64_BOOTCRT_JUMPADDR  0x00AA

/* other external references */
extern uint8_t _bootcrt_65_start;
extern uint8_t _bootcrt_65_end;

cart_def boot_cart = { 0x00, (void *)0, 0x1000, CART_TYPE_8K | CART_REU | CART_RAM };

static void initBootCart(void *object, void *param)
{
    int size = (int)&_bootcrt_65_end - (int)&_bootcrt_65_start;
    boot_cart.custom_addr = new uint8_t[8192];
    boot_cart.length = size;
    memcpy(boot_cart.custom_addr, &_bootcrt_65_start, size);
    printf("%d bytes copied into boot_cart.\n", size);
}
InitFunction bootCart_initializer(initBootCart, NULL, NULL);

static unsigned char eapiOrg[768] = { 0x65, 0x61, 0x70, 0x69, 0xc1, 0x4d, 0x2f, 0xcd, 0x32, 0x39, 0xc6, 0x30, 0x34, 0x30, 0x20,
        0xd6, 0x31, 0x2e, 0x34, 0x00, 0x08, 0x78, 0xa5, 0x4b, 0x48, 0xa5, 0x4c, 0x48, 0xa9, 0x60, 0x85, 0x4b, 0x20, 0x4b, 0x00,
        0xba, 0xbd, 0x00, 0x01, 0x85, 0x4c, 0xca, 0xbd, 0x00, 0x01, 0x85, 0x4b, 0x18, 0x90, 0x70, 0x4c, 0x67, 0x01, 0x4c, 0xa4,
        0x01, 0x4c, 0x39, 0x02, 0x4c, 0x40, 0x02, 0x4c, 0x44, 0x02, 0x4c, 0x4e, 0x02, 0x4c, 0x58, 0x02, 0x4c, 0x8e, 0x02, 0x4c,
        0xd9, 0x02, 0x4c, 0xd9, 0x02, 0x8d, 0x02, 0xde, 0xa9, 0xaa, 0x8d, 0x55, 0x85, 0xa9, 0x55, 0x8d, 0xaa, 0x82, 0xa9, 0xa0,
        0x8d, 0x55, 0x85, 0xad, 0xf2, 0xdf, 0x8d, 0x00, 0xde, 0xa9, 0x00, 0x8d, 0xff, 0xff, 0xa2, 0x07, 0x8e, 0x02, 0xde, 0x60,
        0x8d, 0x02, 0xde, 0xa9, 0xaa, 0x8d, 0x55, 0xe5, 0xa9, 0x55, 0x8d, 0xaa, 0xe2, 0xa9, 0xa0, 0x8d, 0x55, 0xe5, 0xd0, 0xdb,
        0xa2, 0x55, 0x8e, 0xe3, 0xdf, 0x8c, 0xe4, 0xdf, 0xa2, 0x85, 0x8e, 0x02, 0xde, 0x8d, 0xff, 0xff, 0x4c, 0xbb, 0xdf, 0xad,
        0xff, 0xff, 0x60, 0xcd, 0xff, 0xff, 0x60, 0xa2, 0x6f, 0xa0, 0x7f, 0xb1, 0x4b, 0x9d, 0x80, 0xdf, 0xdd, 0x80, 0xdf, 0xd0,
        0x21, 0x88, 0xca, 0x10, 0xf2, 0xa2, 0x00, 0xe8, 0x18, 0xbd, 0x80, 0xdf, 0x65, 0x4b, 0x9d, 0x80, 0xdf, 0xe8, 0xbd, 0x80,
        0xdf, 0x65, 0x4c, 0x9d, 0x80, 0xdf, 0xe8, 0xe0, 0x1e, 0xd0, 0xe8, 0x18, 0x90, 0x06, 0xa9, 0x01, 0x8d, 0xb9, 0xdf, 0x38,
        0x68, 0x85, 0x4c, 0x68, 0x85, 0x4b, 0xb0, 0x48, 0xa9, 0xaa, 0xa0, 0xe5, 0x20, 0xd5, 0xdf, 0xa0, 0x85, 0x20, 0xd5, 0xdf,
        0xa9, 0x55, 0xa2, 0xaa, 0xa0, 0xe2, 0x20, 0xd7, 0xdf, 0xa2, 0xaa, 0xa0, 0x82, 0x20, 0xd7, 0xdf, 0xa9, 0x90, 0xa0, 0xe5,
        0x20, 0xd5, 0xdf, 0xa0, 0x85, 0x20, 0xd5, 0xdf, 0xad, 0x00, 0xa0, 0x8d, 0xf1, 0xdf, 0xae, 0x01, 0xa0, 0x8e, 0xb9, 0xdf,
        0xc9, 0x01, 0xd0, 0x06, 0xe0, 0xa4, 0xd0, 0x02, 0xf0, 0x0c, 0xc9, 0x20, 0xd0, 0x39, 0xe0, 0xe2, 0xd0, 0x35, 0xf0, 0x02,
        0xb0, 0x50, 0xad, 0x00, 0x80, 0xae, 0x01, 0x80, 0xc9, 0x01, 0xd0, 0x06, 0xe0, 0xa4, 0xd0, 0x02, 0xf0, 0x08, 0xc9, 0x20,
        0xd0, 0x19, 0xe0, 0xe2, 0xd0, 0x15, 0xa0, 0x3f, 0x8c, 0x00, 0xde, 0xae, 0x02, 0x80, 0xd0, 0x13, 0xae, 0x02, 0xa0, 0xd0,
        0x12, 0x88, 0x10, 0xf0, 0x18, 0x90, 0x12, 0xa9, 0x02, 0xd0, 0x0a, 0xa9, 0x03, 0xd0, 0x06, 0xa9, 0x04, 0xd0, 0x02, 0xa9,
        0x05, 0x8d, 0xb9, 0xdf, 0x38, 0xa9, 0x00, 0x8d, 0x00, 0xde, 0xa0, 0xe0, 0xa9, 0xf0, 0x20, 0xd7, 0xdf, 0xa0, 0x80, 0x20,
        0xd7, 0xdf, 0xad, 0xb9, 0xdf, 0xb0, 0x08, 0xae, 0xf1, 0xdf, 0xa0, 0x40, 0x28, 0x18, 0x60, 0x28, 0x38, 0x60, 0x8d, 0xb7,
        0xdf, 0x8e, 0xb9, 0xdf, 0x8e, 0xed, 0xdf, 0x8c, 0xba, 0xdf, 0x08, 0x78, 0x98, 0x29, 0xbf, 0x8d, 0xee, 0xdf, 0xa9, 0x00,
        0x8d, 0x00, 0xde, 0xa9, 0x85, 0xc0, 0xe0, 0x90, 0x05, 0x20, 0xc1, 0xdf, 0xb0, 0x03, 0x20, 0x9e, 0xdf, 0xa2, 0x14, 0x20,
        0xec, 0xdf, 0xf0, 0x06, 0xca, 0xd0, 0xf8, 0x18, 0x90, 0x63, 0xad, 0xf2, 0xdf, 0x8d, 0x00, 0xde, 0x18, 0x90, 0x72, 0x8d,
        0xb7, 0xdf, 0x8e, 0xb9, 0xdf, 0x8c, 0xba, 0xdf, 0x08, 0x78, 0x98, 0xc0, 0x80, 0xf0, 0x04, 0xa0, 0xe0, 0xa9, 0xa0, 0x8d,
        0xee, 0xdf, 0xc8, 0xc8, 0xc8, 0xc8, 0xc8, 0xa9, 0xaa, 0x20, 0xd5, 0xdf, 0xa9, 0x55, 0xa2, 0xaa, 0x88, 0x88, 0x88, 0x20,
        0xd7, 0xdf, 0xa9, 0x80, 0xc8, 0xc8, 0xc8, 0x20, 0xd5, 0xdf, 0xa9, 0xaa, 0x20, 0xd5, 0xdf, 0xa9, 0x55, 0xa2, 0xaa, 0x88,
        0x88, 0x88, 0x20, 0xd7, 0xdf, 0xad, 0xb7, 0xdf, 0x8d, 0x00, 0xde, 0xa2, 0x00, 0x8e, 0xed, 0xdf, 0x88, 0x88, 0xa9, 0x30,
        0x20, 0xd7, 0xdf, 0xa9, 0xff, 0xaa, 0xa8, 0xd0, 0x24, 0xad, 0xf2, 0xdf, 0x8d, 0x00, 0xde, 0xa0, 0x80, 0xa9, 0xf0, 0x20,
        0xd7, 0xdf, 0xa0, 0xe0, 0xa9, 0xf0, 0x20, 0xd7, 0xdf, 0x28, 0x38, 0xb0, 0x02, 0x28, 0x18, 0xac, 0xba, 0xdf, 0xae, 0xb9,
        0xdf, 0xad, 0xb7, 0xdf, 0x60, 0x20, 0xec, 0xdf, 0xf0, 0x09, 0xca, 0xd0, 0xf8, 0x88, 0xd0, 0xf5, 0x18, 0x90, 0xce, 0xad,
        0xf2, 0xdf, 0x8d, 0x00, 0xde, 0x18, 0x90, 0xdd, 0x8d, 0xf2, 0xdf, 0x8d, 0x00, 0xde, 0x60, 0xad, 0xf2, 0xdf, 0x60, 0x8d,
        0xf3, 0xdf, 0x8e, 0xe9, 0xdf, 0x8c, 0xea, 0xdf, 0x60, 0x8e, 0xf4, 0xdf, 0x8c, 0xf5, 0xdf, 0x8d, 0xf6, 0xdf, 0x60, 0xad,
        0xf2, 0xdf, 0x8d, 0x00, 0xde, 0x20, 0xe8, 0xdf, 0x8d, 0xb7, 0xdf, 0x8e, 0xf0, 0xdf, 0x8c, 0xf1, 0xdf, 0xa9, 0x00, 0x8d,
        0xba, 0xdf, 0xf0, 0x3b, 0xad, 0xf4, 0xdf, 0xd0, 0x10, 0xad, 0xf5, 0xdf, 0xd0, 0x08, 0xad, 0xf6, 0xdf, 0xf0, 0x0b, 0xce,
        0xf6, 0xdf, 0xce, 0xf5, 0xdf, 0xce, 0xf4, 0xdf, 0x90, 0x45, 0x38, 0xb0, 0x42, 0x8d, 0xb7, 0xdf, 0x8e, 0xf0, 0xdf, 0x8c,
        0xf1, 0xdf, 0xae, 0xe9, 0xdf, 0xad, 0xea, 0xdf, 0xc9, 0xa0, 0x90, 0x02, 0x09, 0x40, 0xa8, 0xad, 0xb7, 0xdf, 0x20, 0x80,
        0xdf, 0xb0, 0x24, 0xee, 0xe9, 0xdf, 0xd0, 0x19, 0xee, 0xea, 0xdf, 0xad, 0xf3, 0xdf, 0x29, 0xe0, 0xcd, 0xea, 0xdf, 0xd0,
        0x0c, 0xad, 0xf3, 0xdf, 0x0a, 0x0a, 0x0a, 0x8d, 0xea, 0xdf, 0xee, 0xf2, 0xdf, 0x18, 0xad, 0xba, 0xdf, 0xf0, 0xa1, 0xac,
        0xf1, 0xdf, 0xae, 0xf0, 0xdf, 0xad, 0xb7, 0xdf, 0x60, 0xff, 0xff, 0xff, 0xff };

static inline uint16_t le2cpu(uint16_t p)
{
#if NIOS
	return p;
#else
	uint16_t out = (p >> 8) | (p << 8);
	return out;
#endif
}

C64_Subsys::C64_Subsys(C64 *machine)  : SubSystem(SUBSYSID_C64) {
	taskHandle = 0;
	c64 = machine;
	fm = FileManager :: getFileManager();
	taskHandle = 0;

	taskCategory = TasksCollection :: getCategory("C64 Machine", SORT_ORDER_C64);
}

C64_Subsys::~C64_Subsys() {
    if (taskHandle) {
    	vTaskDelete(taskHandle);
    }

}

void C64_Subsys :: create_task_items(void)
{
    myActions.reset    = new Action("Reset C64", SUBSYSID_C64, MENU_C64_RESET);
    myActions.reboot   = new Action("Reboot C64", SUBSYSID_C64, MENU_C64_REBOOT);
    myActions.off      = new Action("Power OFF", SUBSYSID_C64, MENU_C64_POWEROFF);
    myActions.pause    = new Action("Pause",  SUBSYSID_C64, MENU_C64_PAUSE);
    myActions.resume   = new Action("Resume", SUBSYSID_C64, MENU_C64_RESUME);
    myActions.savereu  = new Action("Save REU Memory", SUBSYSID_C64, MENU_C64_SAVEREU);
    myActions.savemem  = new Action("Save C64 Memory", SUBSYSID_C64, MENU_U64_SAVERAM);
    myActions.savemod  = new Action("Save Module Memory", SUBSYSID_C64, MENU_C64_SAVEMODULE);
    myActions.save_ef  = new Action("Save Easyflash", SUBSYSID_C64, MENU_C64_SAVEEASYFLASH);
    myActions.savemp3a = new Action("Save MP3 Drv A", SUBSYSID_C64, MENU_C64_SAVE_MP3_DRV_A);
    myActions.savemp3b = new Action("Save MP3 Drv B", SUBSYSID_C64, MENU_C64_SAVE_MP3_DRV_B);
    myActions.savemp3c = new Action("Save MP3 Drv C", SUBSYSID_C64, MENU_C64_SAVE_MP3_DRV_C);
    myActions.savemp3d = new Action("Save MP3 Drv D", SUBSYSID_C64, MENU_C64_SAVE_MP3_DRV_D);

    taskCategory->append(myActions.reset);
    taskCategory->append(myActions.reboot);
#if U64
    taskCategory->append(myActions.off);
#endif
#if DEVELOPER > 0
    taskCategory->append(myActions.pause);
    taskCategory->append(myActions.resume);
#endif
#if U64
    taskCategory->append(myActions.savemem);
#endif
    taskCategory->append(myActions.savereu);
//    taskCategory->append(myActions.savemod);
    taskCategory->append(myActions.save_ef);
    taskCategory->append(myActions.savemp3a);
    taskCategory->append(myActions.savemp3b);
    taskCategory->append(myActions.savemp3c);
    taskCategory->append(myActions.savemp3d);
}

void C64_Subsys :: update_task_items(bool writablePath, Path *p)
{
    if (C64 :: isMP3RamDrive(0) > 0) myActions.savemp3a->show(); else myActions.savemp3a->hide();
    if (C64 :: isMP3RamDrive(1) > 0) myActions.savemp3b->show(); else myActions.savemp3b->hide();
    if (C64 :: isMP3RamDrive(2) > 0) myActions.savemp3c->show(); else myActions.savemp3c->hide();
    if (C64 :: isMP3RamDrive(3) > 0) myActions.savemp3d->show(); else myActions.savemp3d->hide();
    if (C64_CARTRIDGE_TYPE == CART_TYPE_EASY_FLASH) {
        myActions.save_ef->show();
    } else {
        myActions.save_ef->hide();
    }

    if (writablePath) {
#if U64
        myActions.savemem  ->enable();
#endif
        myActions.savereu  ->enable();
        myActions.savemod  ->enable();
        myActions.save_ef  ->enable();
        myActions.savemp3a ->enable();
        myActions.savemp3b ->enable();
        myActions.savemp3c ->enable();
        myActions.savemp3d ->enable();
    } else {
#if U64
        myActions.savemem  ->disable();
#endif
        myActions.savereu  ->disable();
        myActions.savemod  ->disable();
        myActions.save_ef  ->disable();
        myActions.savemp3a ->disable();
        myActions.savemp3b ->disable();
        myActions.savemp3c ->disable();
        myActions.savemp3d ->disable();
    }
}

/*
void C64_Subsys :: poll(void *a)
{
	C64 *c64 = (C64 *)a;

	static uint8_t button_prev;
	while(1) {
		uint8_t buttons = ioRead8(ITU_BUTTON_REG) & ITU_BUTTONS;
		if((buttons & ~button_prev) & ITU_BUTTON1) {
			c64->buttonPushSeen = true;
		}
		button_prev = buttons;
		vTaskDelay(5);
	}
}
*/

void C64_Subsys :: restoreCart(void)
{
	for (int i=0; (i<500) && (C64_CARTRIDGE_ACTIVE); i++) {
		vTaskDelay(2);
	}
	if (C64_CARTRIDGE_ACTIVE) {
	    printf("Error.. cart did not get disabled.\n");
	    c64->init_cartridge(); // hard reset
	} else {
	    printf("Cart got disabled, now restoring.\n");
	    c64->set_cartridge(NULL);
	}
}


int C64_Subsys :: executeCommand(SubsysCommand *cmd)
{
	File *f = 0;
	FRESULT res;
	CachedTreeNode *po;
	uint32_t transferred;
    int ram_size;
    char buffer[64] = "memory";
    uint8_t *pb;

    switch(cmd->functionID) {
    case C64_PUSH_BUTTON:
        c64->setButtonPushed();
        break;
    case C64_UNFREEZE:
		if(c64->client) { // we can't execute this yet
			c64->client->release_host(); // disconnect from user interface
			c64->client = 0;
		}
		c64->unfreeze();
    	break;
    case MENU_C64_RESET:
		if(c64->client) { // we can't execute this yet
			c64->client->release_host(); // disconnect from user interface
			c64->client = 0;
		}
		c64->unfreeze();
		c64->reset();
		break;

    case MENU_C64_PAUSE:
        c64->stop(false);
        break;

    case MENU_C64_RESUME:
        c64->resume();
        break;

    case MENU_C64_POWEROFF:
        U64_POWER_REG = 0x2B;
        U64_POWER_REG = 0xB2;
        U64_POWER_REG = 0x2B;
        U64_POWER_REG = 0xB2;
		break;

    case MENU_C64_REBOOT:
		if(c64->client) { // we can't execute this yet
			c64->client->release_host(); // disconnect from user interface
			c64->client = 0;
		}
		c64->start_cartridge(NULL, false);
		break;

    case C64_START_CART:
		if(c64->client) { // we can't execute this yet
			c64->client->release_host(); // disconnect from user interface
			c64->client = 0;
		}
		c64->start_cartridge((cart_def *)cmd->mode, false);
		break;

    case MENU_C64_SAVEREU:
        ram_size = 128 * 1024;
        ram_size <<= c64->cfg->get_value(CFG_C64_REU_SIZE);
        
        if(cmd->user_interface->string_box("Save REU memory as..", buffer, 22) > 0) {
            fix_filename(buffer);
            set_extension(buffer, ".reu", 32);

            res = create_file_ask_if_exists(fm, cmd->user_interface, cmd->path.c_str(), buffer, &f);
            if(res == FR_OK) {
                printf("Opened file successfully.\n");
                
                uint32_t reu_sizes[] =
                    { 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000, 0x1000000 };

                uint32_t current_reu_size = reu_sizes[c64->cfg->get_value(CFG_C64_REU_SIZE)];
                uint32_t sectors = (current_reu_size >> 9);
                uint32_t secs_per_step = (sectors + 31) >> 5;
				uint32_t bytes_per_step = secs_per_step << 9;
                uint32_t bytes_written;
                uint32_t remain = current_reu_size;
				uint8_t *src;
                transferred = 0;
                
                cmd->user_interface->show_progress("Saving REU file..", 32);
                src = (uint8_t *)REU_MEMORY_BASE;
                
                while(remain != 0) {
                    f->write(src, bytes_per_step, &bytes_written);
                    transferred += bytes_written;
                    cmd->user_interface->update_progress(NULL, 1);
                    remain -= bytes_per_step;
                    src += bytes_per_step;
                }
                cmd->user_interface->hide_progress();                
                
                printf("written: %d...", transferred);
                sprintf(buffer, "Bytes saved: %d ($%8x)", transferred, transferred);
                cmd->user_interface->popup(buffer, BUTTON_OK);
                
                fm->fclose(f);
            } else {
                printf("Couldn't open file..\n");
                cmd->user_interface->popup(FileSystem :: get_error_string(res), BUTTON_OK);
            }
        }
        break;

        case MENU_C64_SAVEMODULE:
            ram_size = 1024 * 1024;

            res = create_file_ask_if_exists(fm, cmd->user_interface, cmd->path.c_str(), "module.bin", &f);
            if(res == FR_OK) {
                uint32_t mem_addr = ((uint32_t)C64_CARTRIDGE_ROM_BASE) << 16;
                printf("Opened file successfully.\n");
                f->write((void *)mem_addr, ram_size, &transferred);
                printf("written: %d...", transferred);
                fm->fclose(f);
            } else { 
                printf("Couldn't open file..\n");
            }       
            break;
        case MENU_U64_SAVERAM:
            ram_size = 64 * 1024;
            if(cmd->user_interface->string_box("Save RAM as..", buffer, 22) > 0) {
                fix_filename(buffer);
                set_extension(buffer, ".bin", 32);

                res = create_file_ask_if_exists(fm, cmd->user_interface, cmd->path.c_str(), buffer, &f);
                if(res == FR_OK) {
                    printf("Opened file successfully.\n");

                    pb = new uint8_t[ram_size];
                    c64->get_all_memory(pb);

                    uint32_t bytes_written;
                    f->write(pb, ram_size, &bytes_written);

                    printf("written: %d...", bytes_written);
                    sprintf(buffer, "bytes saved: %d ($%8x)", bytes_written, bytes_written);
                    cmd->user_interface->popup(buffer, BUTTON_OK);

                    fm->fclose(f);
                    delete[] pb;
                } else {
                    printf("Couldn't open file..\n");
                    cmd->user_interface->popup(FileSystem :: get_error_string(res), BUTTON_OK);
                }
            }
            break;

        case MENU_C64_SAVEEASYFLASH:
            res = create_file_ask_if_exists(fm, cmd->user_interface, cmd->path.c_str(), "module.crt", &f);
            if(res == FR_OK) {
                uint32_t mem_addr = ((uint32_t)C64_CARTRIDGE_ROM_BASE) << 16;
                printf("Opened file successfully.\n");
                uint8_t header[64] = { 0x43, 0x36, 0x34, 0x20, 0x43, 0x41, 0x52, 0x54, 0x52, 0x49, 0x44, 0x47, 0x45, 0x20, 0x20, 0x20,                    0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45, 0x61,
                             0x73, 0x79, 0x46, 0x6c, 0x61, 0x73, 0x68, 0x20, 0x43, 0x61, 0x72, 0x74, 0x72, 0x69, 0x64, 0x67, 0x65, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
                        
                uint8_t header2[16] = { 0x43, 0x48, 0x49, 0x50, 0x00, 0x00, 0x20, 0x10, 0x00, 0x02, 0x00, 0x00, 0x80, 0x00, 0x20, 0x00 };
                int size = 64;
                f->write((void *)header, size, &transferred);
                printf("written: %d...", transferred);
                for (int i=0; i<64; i++) {
                    header2[11] = i;
                    header2[12] = 0x80;
                    size = 16;
                    f->write((void *)header2, size, &transferred);
                    printf("written: %d...", transferred);
                    size = 8192;
                    f->write((void *)(mem_addr+8192*i), size, &transferred);
                    printf("written: %d...", transferred);
                    header2[12] = 0xA0;
                    size = 16;
                    f->write((void *)header2, size, &transferred);
                    printf("written: %d...", transferred);
                    char* eapi = (char*)(mem_addr + 512*1024 + 0x1800);
                    if (i == 0 && eapi[0] == 0x65 && eapi[1] == 0x61 && eapi[2] == 0x70 && eapi[3] == 0x69) {
                        size = 0x1800;
                        f->write((void *)(mem_addr+512*1024), size, &transferred); printf("written: %d...", transferred);
                        size = 0x300;
                        f->write(eapiOrg, size, &transferred); printf("written: %d...", transferred);
                        size = 0x500;                        f->write((void *)(mem_addr+512*1024+6144+768), size, &transferred);                        printf("written: %d...", transferred);
                    } else {
                        size = 8192; f->write((void *)(mem_addr+512*1024+8192*i), size, &transferred);                        printf("written: %d...", transferred);
                    }
                }
                fm->fclose(f);
            } else {
                printf("Couldn't open file..\n");
            }
            break;

    case MENU_C64_SAVEFLASH: // doesn't belong here, but i need it fast
        res = create_file_ask_if_exists(fm, cmd->user_interface, cmd->path.c_str(), "flash_dump.bin", &f);

        if(res == FR_OK) {
            int pages = c64->flash->get_number_of_pages();
            int page_size = c64->flash->get_page_size();
            uint8_t *page_buf = new uint8_t[page_size];
            for(int i=0;i<pages;i++) {
                c64->flash->read_page(i, page_buf);
                f->write(page_buf, page_size, &transferred);
            }
            delete[] page_buf;
            fm->fclose(f);
        } else {
            printf("Couldn't open file..\n");
        }
        break;

    case C64_DMA_LOAD:
    	res = fm->fopen(cmd->path.c_str(), cmd->filename.c_str(), FA_READ, &f);
        if(res == FR_OK) {
        	dma_load(f, NULL, 0, cmd->filename.c_str(), cmd->mode, c64->cfg->get_value(CFG_C64_DMA_ID));
        	fm->fclose(f);
        }
    	break;

#ifndef RECOVERYAPP
    case C64_DMA_LOAD_MNT:
        res = fm->fopen(cmd->path.c_str(), cmd->filename.c_str(), FA_READ, &f);
        if(res == FR_OK) {
            dma_load(f, NULL, 0, cmd->filename.c_str(), cmd->mode, c1541_A->get_current_iec_address());
            fm->fclose(f);
        }
        break;
#endif
    case C64_DMA_LOAD_RAW:
    	res = fm->fopen(cmd->path.c_str(), cmd->filename.c_str(), FA_READ, &f);
        if(res == FR_OK) {
        	dma_load_raw(f);
        	fm->fclose(f);
        }
    	break;
    case C64_DRIVE_LOAD:
    	dma_load(0, NULL, 0, cmd->filename.c_str(), cmd->mode, cmd->path.c_str()[0] & 0x1F);
    	break;
    case C64_DMA_BUFFER:
    	dma_load(0, (const uint8_t *)cmd->buffer, cmd->bufferSize, cmd->filename.c_str(), cmd->mode, c64->cfg->get_value(CFG_C64_DMA_ID));
    	break;
    case C64_DMA_RAW:
    	dma_load_raw_buffer((uint16_t)cmd->mode, (const uint8_t *)cmd->buffer, cmd->bufferSize);
    	break;
    case C64_STOP_COMMAND:
		c64->stop(false);
		break;
    case MENU_C64_HARD_BOOT:
		c64->flash->reboot(0);
		break;
    case C64_READ_FLASH:
        switch (cmd->mode) {
            case FLASH_CMD_PAGESIZE:
                *(int*)cmd->buffer = c64->flash->get_page_size();
                break;
            case FLASH_CMD_NOPAGES:
                *(int*)cmd->buffer = c64->flash->get_number_of_pages();
                break;
            default:
                if (cmd->bufferSize >= c64->flash->get_page_size())
                    c64->flash->read_page(cmd->mode - FLASH_CMD_GETPAGE, cmd->buffer);
        }
        break;
    case C64_SET_KERNAL:
                c64->enable_kernal( (uint8_t*) cmd->buffer );
                // c64->reset();
    	        break;
    case MENU_C64_SAVE_MP3_DRV_A:
    case MENU_C64_SAVE_MP3_DRV_B:
    case MENU_C64_SAVE_MP3_DRV_C:
    case MENU_C64_SAVE_MP3_DRV_D:
        {
            int devNo = cmd->functionID - MENU_C64_SAVE_MP3_DRV_A;
            int ftype = C64::isMP3RamDrive(devNo);
            uint8_t* reu = (uint8_t *) (REU_MEMORY_BASE);
            int expSize = 1;
            char* extension = 0;

            if (ftype == 1541) {
                expSize = 174848;
                extension = ".D64";
            }
            if (ftype == 1571) {
                expSize = 2 * 174848;
                extension = ".D71";
            }
            if (ftype == 1581) {
                expSize = 819200;
                extension = ".D81";
            }
            if (ftype == DRVTYPE_MP3_DNP) {
                expSize = C64::getSizeOfMP3NativeRamdrive(devNo);
                extension = ".DNP";
            }

            uint8_t ramBase = reu[0x7dc7 + devNo];
            uint8_t* srcAddr = reu + (((uint32_t) ramBase) << 16);

            if (cmd->user_interface->string_box("Save RAMDISK as..", buffer, 22) > 0) {
                fix_filename(buffer);
                set_extension(buffer, extension, 32);

                res = create_file_ask_if_exists(fm, cmd->user_interface, cmd->path.c_str(), buffer, &f);
                if (res == FR_OK) {
                    printf("Opened file successfully.\n");
                    uint32_t bytes_written;

                    if (ftype == 1571) {
                        f->write(srcAddr, expSize / 2, &bytes_written);
                        uint32_t tmp;
                        f->write(srcAddr + 700 * 256, expSize / 2, &tmp);
                        bytes_written += tmp;
                    } else {
                        f->write(srcAddr, expSize, &bytes_written);
                    }
                    printf("written: %d...", bytes_written);
                    sprintf(buffer, "bytes saved: %d ($%8x)", bytes_written, bytes_written);
                    cmd->user_interface->popup(buffer, BUTTON_OK);

                    fm->fclose(f);
                } else {
                    printf("Couldn't open file..\n");
                    cmd->user_interface->popup(FileSystem::get_error_string(res), BUTTON_OK);
                }
            }
        }
        break;

    default:
		break;
	}

    return 0;
}

int C64_Subsys :: dma_load_raw(File *f)
{
	bool i_stopped_it = false;
	if (c64->client) {
    	c64->client->release_host(); // disconnect from user interface
    	c64->release_ownership();
	}
	if(!c64->isFrozen) {
		c64->stop(false);
        C64_MODE = MODE_NORMAL;
		i_stopped_it = true;
	}
	int bytes = load_file_dma(f, 0);

	if (i_stopped_it) {
		c64->resume();
	}
	return bytes;
}

int C64_Subsys :: dma_load_raw_buffer(uint16_t offset, const uint8_t *buffer, int length)
{
	bool i_stopped_it = false;
	if (c64->client) {
    	c64->client->release_host(); // disconnect from user interface
    	c64->release_ownership();
	}
	if(!c64->isFrozen) {
		c64->stop(false);
		i_stopped_it = true;
	}

	volatile uint8_t *dest = (volatile uint8_t *)(C64_MEMORY_BASE + offset);

	memcpy((void *)dest, buffer, length);

	if (i_stopped_it) {
		c64->resume();
	}
	return length;
}

int C64_Subsys :: dma_load(File *f, const uint8_t *buffer, const int bufferSize,
		const char *name, uint8_t run_code, uint8_t drv, uint16_t reloc)
{
	// prepare DMA load
    if(c64->client) { // we are locked by a client, likely: user interface
    	c64->client->release_host(); // disconnect from user interface
    	c64->client = 0;
	}
    if(!c64->isFrozen) {
    	c64->stop(false);
    }

    C64_POKE(C64_BOOTCRT_RUNCODE, run_code);

    C64_POKE(C64_BOOTCRT_DOSYNC, (c64->cfg->get_value(CFG_C64_DO_SYNC) == 1) ? 1 : 0);

    CbmFileName cbm;
    cbm.init(name);
    const char *p = cbm.getName();
    for(int i=0; i<16; i++) {
        C64_POKE(C64_BOOTCRT_NAME+i, p[i]);
    }
    C64_POKE(C64_BOOTCRT_NAMELEN, strlen(p));

    C64_POKE(C64_BOOTCRT_HANDSHAKE, 0x80); // initial boot cart handshake

    boot_cart.custom_addr = (void *)&_bootcrt_65_start;
    c64->start_cartridge(&boot_cart, false);

	// perform DMA load
    wait_ms(100);

	// handshake with boot cart
    c64->stop(false);

    C64_POKE(C64_BOOTCRT_DRIVENUM, drv);    // drive number for printout

	C64_POKE(C64_BOOTCRT_HANDSHAKE, 0x40);  // signal cart ready for DMA load

	if ( !(run_code & RUNCODE_REAL_BIT) ) {
        int timeout = 0;
        while(C64_PEEK(2) != 0x01) {
        	c64->resume();
            timeout++;
            if(timeout == 60) {
                c64->init_cartridge();
                return -1;
            }
            wait_ms(25);
            printf("_");
            c64->stop(false);
        }
        if (f) {
        	load_file_dma(f, 0);
        } else {
            // If we need to jump, the first two bytes are the jump address
            if (run_code & RUNCODE_JUMP_BIT) {
            	C64_POKE(C64_BOOTCRT_JUMPADDR, buffer[0]);
            	C64_POKE(C64_BOOTCRT_JUMPADDR+1, buffer[1]);
            	load_buffer_dma(buffer+2, bufferSize-2, 0);
            } else {
            	load_buffer_dma(buffer, bufferSize, 0);
            }
        }

        C64_POKE(C64_BOOTCRT_HANDSHAKE, 0); // signal DMA load done
        C64_POKE(C64_BOOTCRT_RUNCODE, run_code);
        C64_POKE(0x00BA, drv);    // fix drive number
	}

	printf("Resuming..\n");
    c64->resume();

    restoreCart();
    return 0;
}

int C64_Subsys :: load_file_dma(File *f, uint16_t reloc)
{
	uint32_t dma_load_buffer[128];
    uint32_t transferred = 0;
    uint16_t load_address = 0;
    uint8_t  *dma_load_buffer_b;
    dma_load_buffer_b = (uint8_t *)dma_load_buffer;

    if (f) {
        f->read(&load_address, 2, &transferred);
        if(transferred != 2) {
            printf("Can't read from file..\n");
            return -1;
        }
        load_address = le2cpu(load_address); // make sure we can interpret the word correctly (endianness)
        printf("Load address: %4x...", load_address);
    }
    if(reloc) {
    	load_address = reloc;
    	printf(" -> %4x ..", load_address);
    }
    int max_length = 65536 - int(load_address); // never exceed $FFFF
    int block = 510;
    printf("Now loading...");
	volatile uint8_t *dest = (volatile uint8_t *)(C64_MEMORY_BASE + load_address);

	/* Now actually load the file */
	int total_trans = 0;
	while (max_length > 0) {
		FRESULT fres = f->read(dma_load_buffer, block, &transferred);
		if (fres != FR_OK) {
			printf("Error reading from file. %s\n", FileSystem :: get_error_string(fres));
			return -1;
		}
		//printf("[%d]", transferred);
		total_trans += transferred;
		volatile uint8_t *d = dest;
		for (int i=0;i<transferred;i++) {
			*(d++) = dma_load_buffer_b[i];
		}
		if (transferred < block) {
			break;
		}
		dest += transferred;
		max_length -= transferred;
		block = (max_length > 512) ? 512 : max_length;
	}
	uint16_t end_address = load_address + total_trans;
	printf("DMA load complete: $%4x-$%4x\n", load_address, end_address);

	// The following pokes are documented in the C64 documentation and are not
	// Ultimate specific.
	C64_POKE(0x002D, end_address);
	C64_POKE(0x002E, end_address >> 8);
	C64_POKE(0x002F, end_address);
	C64_POKE(0x0030, end_address >> 8);
	C64_POKE(0x0031, end_address);
	C64_POKE(0x0032, end_address >> 8);
	C64_POKE(0x00AE, end_address);
	C64_POKE(0x00AF, end_address >> 8);

	C64_POKE(0x0090, 0x40); // Load status
	C64_POKE(0x0035, 0);    // FRESPC
	C64_POKE(0x0036, 0xA0);

	return total_trans;
}

int C64_Subsys :: load_buffer_dma(const uint8_t *buffer, const int bufferSize, uint16_t reloc)
{
    uint16_t load_address = 0;

    load_address = (uint16_t)buffer[0] | ((uint16_t)buffer[1]) << 8;
	printf("Load address: %4x...", load_address);

	if (reloc) {
    	load_address = reloc;
    	printf(" -> %4x ..", load_address);
    }
	volatile uint8_t *dest = (volatile uint8_t *)(C64_MEMORY_BASE + load_address);

	memcpy((void *)dest, buffer + 2, bufferSize - 2);

	uint16_t end_address = load_address + bufferSize - 2;
	printf("DMA load complete: $%4x-$%4x\n", load_address, end_address);

    // The following pokes are documented in the C64 documentation and are not
    // Ultimate specific.
	C64_POKE(0x002D, end_address);
	C64_POKE(0x002E, end_address >> 8);
	C64_POKE(0x002F, end_address);
	C64_POKE(0x0030, end_address >> 8);
	C64_POKE(0x0031, end_address);
	C64_POKE(0x0032, end_address >> 8);
	C64_POKE(0x00AE, end_address);
	C64_POKE(0x00AF, end_address >> 8);

	C64_POKE(0x0090, 0x40); // Load status
	C64_POKE(0x0035, 0);    // FRESPC
	C64_POKE(0x0036, 0xA0);

	return bufferSize - 2;
}

bool C64_Subsys :: write_vic_state(File *f)
{
    uint32_t transferred;
    uint8_t mode = C64_MODE;
    C64_MODE = 0;

    // find bank
    uint8_t bank = 3 - (c64->cia_backup[1] & 0x03);
    uint32_t addr = C64_MEMORY_BASE + (uint32_t(bank) << 14);
    if(bank == 0) {
        f->write((uint8_t *)C64_MEMORY_BASE, 0x0400, &transferred);
        f->write(c64->screen_backup, 0x0400, &transferred);
        f->write(c64->ram_backup, 0x0800, &transferred);
        f->write((uint8_t *)(C64_MEMORY_BASE + 0x1000), 0x3000, &transferred);
    } else {
        f->write((uint8_t *)addr, 16384, &transferred);
    }
    f->write(c64->color_backup, 0x0400, &transferred);
    f->write(c64->vic_backup, NUM_VICREGS, &transferred);

    C64_MODE = mode;
    return true;
}
