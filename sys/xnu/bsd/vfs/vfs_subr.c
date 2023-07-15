 *
#include <kern/kalloc.h>
#include <sys/reboot.h>
#include <nfs/nfs.h>
#include <kern/smr.h>
#include <libkern/coreanalytics/coreanalytics.h>
#include <os/atomic_private.h>
#if defined(XNU_TARGET_OS_OSX)
#ifdef CONFIG_IOCOUNT_TRACE
static LCK_GRP_DECLARE(vnode_lck_grp, "vnode");
static LCK_ATTR_DECLARE(vnode_lck_attr, 0, 0);
static LCK_GRP_DECLARE(trigger_vnode_lck_grp, "trigger_vnode");
static LCK_ATTR_DECLARE(trigger_vnode_lck_attr, 0, 0);
extern lck_mtx_t mnt_list_mtx_lock;

static KALLOC_TYPE_DEFINE(specinfo_zone, struct specinfo, KT_DEFAULT);

ZONE_DEFINE(vnode_zone, "vnodes",
    sizeof(struct vnode), ZC_NOGC | ZC_ZFREE_CLEARMEM);
#ifdef CONFIG_IOCOUNT_TRACE
static TUNABLE(int, bootarg_vnode_iocount_trace, "vnode_iocount_trace", 0);
static TUNABLE(int, bootarg_uthread_iocount_trace, "uthread_iocount_trace", 0);
#endif /* CONFIG_IOCOUNT_TRACE */
static TUNABLE(bool, bootarg_no_vnode_jetsam, "-no_vnode_jetsam", false);
static TUNABLE(bool, bootarg_no_vnode_drain, "-no_vnode_drain", false);

__options_decl(freeable_vnode_level_t, uint32_t, {
	DEALLOC_VNODE_NONE = 0,
	DEALLOC_VNODE_ONLY_OVERFLOW = 1,
	DEALLOC_VNODE_ALL = 2
});

#if XNU_TARGET_OS_OSX
static TUNABLE(freeable_vnode_level_t, bootarg_vn_dealloc_level, "vn_dealloc_level", DEALLOC_VNODE_NONE);
#else
static TUNABLE(freeable_vnode_level_t, bootarg_vn_dealloc_level, "vn_dealloc_level", DEALLOC_VNODE_ONLY_OVERFLOW);
#endif /* CONFIG_VNDEALLOC */

static freeable_vnode_level_t vn_dealloc_level = DEALLOC_VNODE_NONE;

long  reusablevnodes_max = LONG_MAX;
long  reusablevnodes = 0;
int   deadvnodes_low = 0;
int   deadvnodes_high = 0;
int   numvnodes_min = 0;
int   numvnodes_max = 0;

uint64_t newvnode = 0;
unsigned long newvnode_nodead = 0;

static  int vfs_unmountall_finished = 0;
static  uint64_t vfs_shutdown_last_completion_time;
VFS_SMR_DECLARE;
extern uint32_t nc_smr_enabled;

/*
 * These could be in PlatformSupport but aren't yet
 */
#define PLATFORM_PREBOOT_VOLUME_MOUNT_POINT "/System/Volumes/Preboot"
#define PLATFORM_RECOVERY_VOLUME_MOUNT_POINT "/System/Volumes/Recovery"

#if CONFIG_MOUNT_VM
#define PLATFORM_VM_VOLUME_MOUNT_POINT "/System/Volumes/VM"
#endif
	        reusablevnodes--;    \
	        if (vp->v_listflag & VLIST_NO_REUSE) {        \
	                deadvnodes_noreuse--;        \
	        }        \
	        if (!(vp->v_listflag & VLIST_NO_REUSE)) {        \
	                reusablevnodes--;    \
	        }        \
	        reusablevnodes--;    \
static void vn_laundry_continue(void);
static void wakeup_laundry_thread(void);
static void vnode_smr_free(void *, size_t);

CA_EVENT(freeable_vnodes,
    CA_INT, numvnodes_min,
    CA_INT, numvnodes_max,
    CA_INT, desiredvnodes,
    CA_INT, numvnodes,
    CA_INT, freevnodes,
    CA_INT, deadvnodes,
    CA_INT, freeablevnodes,
    CA_INT, busyvnodes,
    CA_BOOL, threshold_crossed);
static CA_EVENT_TYPE(freeable_vnodes) freeable_vnodes_telemetry;

static bool freeablevnodes_threshold_crossed = false;
	int desiredvnodes_one_percent = desiredvnodes / 100;
	rage_limit = desiredvnodes_one_percent;
	deadvnodes_low = desiredvnodes_one_percent;
	if (deadvnodes_low > 300) {
		deadvnodes_low = 300;
	}
	deadvnodes_high = deadvnodes_low * 2;

	numvnodes_min = numvnodes_max = desiredvnodes;
	if (bootarg_vn_dealloc_level == DEALLOC_VNODE_ONLY_OVERFLOW) {
		numvnodes_max = desiredvnodes * 2;
		vn_dealloc_level = bootarg_vn_dealloc_level;
	} else if (bootarg_vn_dealloc_level == DEALLOC_VNODE_ALL) {
		numvnodes_min = desiredvnodes_one_percent * 40;
		numvnodes_max = desiredvnodes * 2;
		reusablevnodes_max = (desiredvnodes_one_percent * 20) - deadvnodes_low;
		vn_dealloc_level = bootarg_vn_dealloc_level;
	}

	bzero(&freeable_vnodes_telemetry, sizeof(CA_EVENT_TYPE(freeable_vnodes)));
	freeable_vnodes_telemetry.numvnodes_min = numvnodes_min;
	freeable_vnodes_telemetry.numvnodes_max = numvnodes_max;
	freeable_vnodes_telemetry.desiredvnodes = desiredvnodes;

	if (nc_smr_enabled) {
		zone_enable_smr(vnode_zone, VFS_SMR(), &vnode_smr_free);
	}

	kernel_thread_start((thread_continue_t)vn_laundry_continue, NULL, &thread);
	thread_deallocate(thread);
	if (output_target < 0) {
		return EINVAL;
	}

	 * Not taking the buf_mtx as there is little
	 * Not taking the buf_mtx as there is little
#if defined(__x86_64__)
#endif /* defined(__x86_64__) */
#if defined(__x86_64__)
#if defined(__x86_64__)
		vnode_hold(vp);
			vnode_drop(vp);
		vnode_drop(vp);
#if defined(__x86_64__)
		msleep((caddr_t)&mp->mnt_iterref, &mnt_list_mtx_lock, PVFS, "mount_iterdrain", NULL);
	if (nc_smr_enabled) {
		vfs_smr_synchronize();
	}

	zfree(mount_zone, mp);
	mp = zalloc_flags(mount_zone, Z_WAITOK | Z_ZERO);
	/*
	 * Reset any prior "unmounting everything" state.  This handles the
	 * situation where mount root and then unmountall and re-mountroot
	 * a new image (see bsd/kern/imageboot.c).
	 */
	vfs_unmountall_started = vfs_unmountall_finished = 0;
	OSMemoryBarrier();

#if defined(XNU_TARGET_OS_OSX)
#endif /* XNU_TARGET_OS_OSX */
		vfs_rootmountfailed(mp);
static int
cache_purge_callback(mount_t mp, __unused void * arg)
{
	cache_purgevfs(mp);
	return VFS_RETURNED;
}

extern lck_rw_t rootvnode_rw_lock;
extern void set_rootvnode(vnode_t);


static int
mntonname_fixup_callback(mount_t mp, __unused void *arg)
{
	int error = 0;

	if ((strncmp(&mp->mnt_vfsstat.f_mntonname[0], "/", sizeof("/")) == 0) ||
	    (strncmp(&mp->mnt_vfsstat.f_mntonname[0], "/dev", sizeof("/dev")) == 0)) {
		return 0;
	}

	if ((error = vfs_busy(mp, LK_NOWAIT))) {
		printf("vfs_busy failed with %d for %s\n", error, mp->mnt_vfsstat.f_mntonname);
		return -1;
	}

	size_t pathlen = MAXPATHLEN;
	if ((error = vn_getpath_ext(mp->mnt_vnodecovered, NULL, mp->mnt_vfsstat.f_mntonname, &pathlen, VN_GETPATH_FSENTER))) {
		printf("vn_getpath_ext failed with %d for mnt_vnodecovered of %s\n", error, mp->mnt_vfsstat.f_mntonname);
	}

	vfs_unbusy(mp);

	return error;
}

static int
clear_mntk_backs_root_callback(mount_t mp, __unused void *arg)
{
	lck_rw_lock_exclusive(&mp->mnt_rwlock);
	mp->mnt_kern_flag &= ~MNTK_BACKS_ROOT;
	lck_rw_done(&mp->mnt_rwlock);
	return VFS_RETURNED;
}

static int
verify_incoming_rootfs(vnode_t *incoming_rootvnodep, vfs_context_t ctx,
    vfs_switch_root_flags_t flags)
{
	mount_t mp;
	vnode_t tdp;
	vnode_t incoming_rootvnode_with_iocount = *incoming_rootvnodep;
	vnode_t incoming_rootvnode_with_usecount = NULLVP;
	int error = 0;

	if (vnode_vtype(incoming_rootvnode_with_iocount) != VDIR) {
		printf("Incoming rootfs path not a directory\n");
		error = ENOTDIR;
		goto done;
	}

	/*
	 * Before we call VFS_ROOT, we have to let go of the iocount already
	 * acquired, but before doing that get a usecount.
	 */
	vnode_ref_ext(incoming_rootvnode_with_iocount, 0, VNODE_REF_FORCE);
	incoming_rootvnode_with_usecount = incoming_rootvnode_with_iocount;
	vnode_lock_spin(incoming_rootvnode_with_usecount);
	if ((mp = incoming_rootvnode_with_usecount->v_mount)) {
		mp->mnt_crossref++;
		vnode_unlock(incoming_rootvnode_with_usecount);
	} else {
		vnode_unlock(incoming_rootvnode_with_usecount);
		printf("Incoming rootfs root vnode does not have associated mount\n");
		error = ENOTDIR;
		goto done;
	}

	if (vfs_busy(mp, LK_NOWAIT)) {
		printf("Incoming rootfs root vnode mount is busy\n");
		error = ENOENT;
		goto out;
	}

	vnode_put(incoming_rootvnode_with_iocount);
	incoming_rootvnode_with_iocount = NULLVP;

	error = VFS_ROOT(mp, &tdp, ctx);

	if (error) {
		printf("Could not get rootvnode of incoming rootfs\n");
	} else if (tdp != incoming_rootvnode_with_usecount) {
		vnode_put(tdp);
		tdp = NULLVP;
		printf("Incoming rootfs root vnode mount is is not a mountpoint\n");
		error = EINVAL;
		goto out_busy;
	} else {
		incoming_rootvnode_with_iocount = tdp;
		tdp = NULLVP;
	}

	if ((flags & VFSSR_VIRTUALDEV_PROHIBITED) != 0) {
		if (mp->mnt_flag & MNTK_VIRTUALDEV) {
			error = ENODEV;
		}
		if (error) {
			printf("Incoming rootfs is backed by a virtual device; cannot switch to it");
			goto out_busy;
		}
	}

out_busy:
	vfs_unbusy(mp);

out:
	vnode_lock(incoming_rootvnode_with_usecount);
	mp->mnt_crossref--;
	if (mp->mnt_crossref < 0) {
		panic("mount cross refs -ve");
	}
	vnode_unlock(incoming_rootvnode_with_usecount);

done:
	if (incoming_rootvnode_with_usecount) {
		vnode_rele(incoming_rootvnode_with_usecount);
		incoming_rootvnode_with_usecount = NULLVP;
	}

	if (error && incoming_rootvnode_with_iocount) {
		vnode_put(incoming_rootvnode_with_iocount);
		incoming_rootvnode_with_iocount = NULLVP;
	}

	*incoming_rootvnodep = incoming_rootvnode_with_iocount;
	return error;
}

 * vfs_switch_root()
 *
 * Move the current root volume, and put a different volume at the root.
 *
 * incoming_vol_old_path: This is the path where the incoming root volume
 *	is mounted when this function begins.
 * outgoing_vol_new_path: This is the path where the outgoing root volume
 *	will be mounted when this function (successfully) ends.
 *	Note: Do not use a leading slash.
 *
 * Volumes mounted at several fixed points (including /dev) will be preserved
 * at the same absolute path. That means they will move within the folder
 * hierarchy during the pivot operation. For example, /dev before the pivot
 * will be at /dev after the pivot.
 *
 * If any filesystem has MNTK_BACKS_ROOT set, it will be cleared. If the
 * incoming root volume is actually a disk image backed by some other
 * filesystem, it is the caller's responsibility to re-set MNTK_BACKS_ROOT
 * as appropriate.
vfs_switch_root(const char *incoming_vol_old_path,
    const char *outgoing_vol_new_path,
    vfs_switch_root_flags_t flags)
	// grumble grumble
#define countof(x) (sizeof(x) / sizeof(x[0]))
	struct preserved_mount {
		vnode_t pm_rootvnode;
		mount_t pm_mount;
		vnode_t pm_new_covered_vp;
		vnode_t pm_old_covered_vp;
		const char *pm_path;
	};

	vfs_context_t ctx = vfs_context_kernel();
	vnode_t incoming_rootvnode = NULLVP;
	vnode_t outgoing_vol_new_covered_vp = NULLVP;
	vnode_t incoming_vol_old_covered_vp = NULLVP;
	mount_t outgoing = NULL;
	mount_t incoming = NULL;

	struct preserved_mount devfs = { NULLVP, NULL, NULLVP, NULLVP, "dev" };
	struct preserved_mount preboot = { NULLVP, NULL, NULLVP, NULLVP, "System/Volumes/Preboot" };
	struct preserved_mount recovery = { NULLVP, NULL, NULLVP, NULLVP, "System/Volumes/Recovery" };
	struct preserved_mount vm = { NULLVP, NULL, NULLVP, NULLVP, "System/Volumes/VM" };
	struct preserved_mount update = { NULLVP, NULL, NULLVP, NULLVP, "System/Volumes/Update" };
	struct preserved_mount iscPreboot = { NULLVP, NULL, NULLVP, NULLVP, "System/Volumes/iSCPreboot" };
	struct preserved_mount hardware = { NULLVP, NULL, NULLVP, NULLVP, "System/Volumes/Hardware" };
	struct preserved_mount xarts = { NULLVP, NULL, NULLVP, NULLVP, "System/Volumes/xarts" };
	struct preserved_mount factorylogs = { NULLVP, NULL, NULLVP, NULLVP, "FactoryLogs" };
	struct preserved_mount idiags = { NULLVP, NULL, NULLVP, NULLVP, "System/Volumes/Diags" };

	struct preserved_mount *preserved[10];
	preserved[0] = &devfs;
	preserved[1] = &preboot;
	preserved[2] = &recovery;
	preserved[3] = &vm;
	preserved[4] = &update;
	preserved[5] = &iscPreboot;
	preserved[6] = &hardware;
	preserved[7] = &xarts;
	preserved[8] = &factorylogs;
	preserved[9] = &idiags;

	int error;

	printf("%s : shuffling mount points : %s <-> / <-> %s\n", __FUNCTION__, incoming_vol_old_path, outgoing_vol_new_path);

	if (outgoing_vol_new_path[0] == '/') {
		// I should have written this to be more helpful and just advance the pointer forward past the slash
		printf("Do not use a leading slash in outgoing_vol_new_path\n");
		return EINVAL;
	}

	// Set incoming_rootvnode.
	// Find the vnode representing the mountpoint of the new root
	// filesystem. That will be the new root directory.
	error = vnode_lookup(incoming_vol_old_path, 0, &incoming_rootvnode, ctx);
		printf("Incoming rootfs root vnode not found\n");
		error = ENOENT;
		goto done;
	/*
	 * This function drops the icoount and sets the vnode to NULL on error.
	 */
	error = verify_incoming_rootfs(&incoming_rootvnode, ctx, flags);
	if (error) {
		goto done;
	}

	/*
	 * Set outgoing_vol_new_covered_vp.
	 * Find the vnode representing the future mountpoint of the old
	 * root filesystem, inside the directory incoming_rootvnode.
	 * Right now it's at "/incoming_vol_old_path/outgoing_vol_new_path".
	 * soon it will become "/oldrootfs_path_after", which will be covered.
	 */
	error = vnode_lookupat(outgoing_vol_new_path, 0, &outgoing_vol_new_covered_vp, ctx, incoming_rootvnode);
	if (error) {
		printf("Outgoing rootfs path not found, abandoning / switch, error = %d\n", error);
		error = ENOENT;
		goto done;
	}
	if (vnode_vtype(outgoing_vol_new_covered_vp) != VDIR) {
		printf("Outgoing rootfs path is not a directory, abandoning / switch\n");
		error = ENOTDIR;
		goto done;
	}

	/*
	 * Find the preserved mounts - see if they are mounted. Get their root
	 * vnode if they are. If they aren't, leave rootvnode NULL which will
	 * be the signal to ignore this mount later on.
	 *
	 * Also get preserved mounts' new_covered_vp.
	 * Find the node representing the folder "dev" inside the directory newrootvnode.
	 * Right now it's at "/incoming_vol_old_path/dev".
	 * Soon it will become /dev, which will be covered by the devfs mountpoint.
	 */
	for (size_t i = 0; i < countof(preserved); i++) {
		struct preserved_mount *pmi = preserved[i];

		error = vnode_lookupat(pmi->pm_path, 0, &pmi->pm_rootvnode, ctx, rootvnode);
		if (error) {
			printf("skipping preserved mountpoint because not found or error: %d: %s\n", error, pmi->pm_path);
			// not fatal. try the next one in the list.
			continue;
		}
		bool is_mountpoint = false;
		vnode_lock_spin(pmi->pm_rootvnode);
		if ((pmi->pm_rootvnode->v_flag & VROOT) != 0) {
			is_mountpoint = true;
		}
		vnode_unlock(pmi->pm_rootvnode);
		if (!is_mountpoint) {
			printf("skipping preserved mountpoint because not a mountpoint: %s\n", pmi->pm_path);
			vnode_put(pmi->pm_rootvnode);
			pmi->pm_rootvnode = NULLVP;
			// not fatal. try the next one in the list.
			continue;
		}

		error = vnode_lookupat(pmi->pm_path, 0, &pmi->pm_new_covered_vp, ctx, incoming_rootvnode);
		if (error) {
			printf("preserved new mount directory not found or error: %d: %s\n", error, pmi->pm_path);
			error = ENOENT;
			goto done;
		}
		if (vnode_vtype(pmi->pm_new_covered_vp) != VDIR) {
			printf("preserved new mount directory not directory: %s\n", pmi->pm_path);
			error = ENOTDIR;
			goto done;

		printf("will preserve mountpoint across pivot: /%s\n", pmi->pm_path);
	/*
	 * --
	 * At this point, everything has been prepared and all error conditions
	 * have been checked. We check everything we can before this point;
	 * from now on we start making destructive changes, and we can't stop
	 * until we reach the end.
	 * ----
	 */

	/* this usecount is transferred to the mnt_vnodecovered */
	vnode_ref_ext(outgoing_vol_new_covered_vp, 0, VNODE_REF_FORCE);
	/* this usecount is transferred to set_rootvnode */
	vnode_ref_ext(incoming_rootvnode, 0, VNODE_REF_FORCE);


	for (size_t i = 0; i < countof(preserved); i++) {
		struct preserved_mount *pmi = preserved[i];
		if (pmi->pm_rootvnode == NULLVP) {
			continue;
		}

		/* this usecount is transferred to the mnt_vnodecovered */
		vnode_ref_ext(pmi->pm_new_covered_vp, 0, VNODE_REF_FORCE);

		/* The new_covered_vp is a mountpoint from now on. */
		vnode_lock_spin(pmi->pm_new_covered_vp);
		pmi->pm_new_covered_vp->v_flag |= VMOUNT;
		vnode_unlock(pmi->pm_new_covered_vp);
	/* The outgoing_vol_new_covered_vp is a mountpoint from now on. */
	vnode_lock_spin(outgoing_vol_new_covered_vp);
	outgoing_vol_new_covered_vp->v_flag |= VMOUNT;
	vnode_unlock(outgoing_vol_new_covered_vp);
	/*
	 * Identify the mount_ts of the mounted filesystems that are being
	 * manipulated: outgoing rootfs, incoming rootfs, and the preserved
	 * mounts.
	 */
	outgoing = rootvnode->v_mount;
	incoming = incoming_rootvnode->v_mount;
	for (size_t i = 0; i < countof(preserved); i++) {
		struct preserved_mount *pmi = preserved[i];
		if (pmi->pm_rootvnode == NULLVP) {
			continue;
		}

		pmi->pm_mount = pmi->pm_rootvnode->v_mount;
	lck_rw_lock_exclusive(&rootvnode_rw_lock);
	/* Setup incoming as the new rootfs */
	lck_rw_lock_exclusive(&incoming->mnt_rwlock);
	incoming_vol_old_covered_vp = incoming->mnt_vnodecovered;
	incoming->mnt_vnodecovered = NULLVP;
	strlcpy(incoming->mnt_vfsstat.f_mntonname, "/", MAXPATHLEN);
	incoming->mnt_flag |= MNT_ROOTFS;
	lck_rw_done(&incoming->mnt_rwlock);
	/*
	 * The preserved mountpoints will now be moved to
	 * incoming_rootnode/pm_path, and then by the end of the function,
	 * since incoming_rootnode is going to /, the preserved mounts
	 * will be end up back at /pm_path
	 */
	for (size_t i = 0; i < countof(preserved); i++) {
		struct preserved_mount *pmi = preserved[i];
		if (pmi->pm_rootvnode == NULLVP) {
			continue;
		}

		lck_rw_lock_exclusive(&pmi->pm_mount->mnt_rwlock);
		pmi->pm_old_covered_vp = pmi->pm_mount->mnt_vnodecovered;
		pmi->pm_mount->mnt_vnodecovered = pmi->pm_new_covered_vp;
		vnode_lock_spin(pmi->pm_new_covered_vp);
		pmi->pm_new_covered_vp->v_mountedhere = pmi->pm_mount;
		vnode_unlock(pmi->pm_new_covered_vp);
		lck_rw_done(&pmi->pm_mount->mnt_rwlock);
	}

	/*
	 * The old root volume now covers outgoing_vol_new_covered_vp
	 * on the new root volume. Remove the ROOTFS marker.
	 * Now it is to be found at outgoing_vol_new_path
	 */
	lck_rw_lock_exclusive(&outgoing->mnt_rwlock);
	outgoing->mnt_vnodecovered = outgoing_vol_new_covered_vp;
	strlcpy(outgoing->mnt_vfsstat.f_mntonname, "/", MAXPATHLEN);
	strlcat(outgoing->mnt_vfsstat.f_mntonname, outgoing_vol_new_path, MAXPATHLEN);
	outgoing->mnt_flag &= ~MNT_ROOTFS;
	vnode_lock_spin(outgoing_vol_new_covered_vp);
	outgoing_vol_new_covered_vp->v_mountedhere = outgoing;
	vnode_unlock(outgoing_vol_new_covered_vp);
	lck_rw_done(&outgoing->mnt_rwlock);

	if (!(outgoing->mnt_kern_flag & MNTK_VIRTUALDEV) &&
	    (TAILQ_FIRST(&mountlist) == outgoing)) {
		vfs_setmntsystem(outgoing);
	}

	/*
	 * Finally, remove the mount_t linkage from the previously covered
	 * vnodes on the old root volume. These were incoming_vol_old_path,
	 * and each preserved mounts's "/pm_path". The filesystems previously
	 * mounted there have already been moved away.
	 */
	vnode_lock_spin(incoming_vol_old_covered_vp);
	incoming_vol_old_covered_vp->v_flag &= ~VMOUNT;
	incoming_vol_old_covered_vp->v_mountedhere = NULL;
	vnode_unlock(incoming_vol_old_covered_vp);

	for (size_t i = 0; i < countof(preserved); i++) {
		struct preserved_mount *pmi = preserved[i];
		if (pmi->pm_rootvnode == NULLVP) {
			continue;
		}

		vnode_lock_spin(pmi->pm_old_covered_vp);
		pmi->pm_old_covered_vp->v_flag &= ~VMOUNT;
		pmi->pm_old_covered_vp->v_mountedhere = NULL;
		vnode_unlock(pmi->pm_old_covered_vp);
	}

	/*
	 * Clear the name cache since many cached names are now invalid.
	 */
	vfs_iterate(0 /* flags */, cache_purge_callback, NULL);

	/*
	 * Actually change the rootvnode! And finally drop the lock that
	 * prevents concurrent vnode_lookups.
	 */
	set_rootvnode(incoming_rootvnode);
	lck_rw_unlock_exclusive(&rootvnode_rw_lock);

	if (!(incoming->mnt_kern_flag & MNTK_VIRTUALDEV) &&
	    !(outgoing->mnt_kern_flag & MNTK_VIRTUALDEV)) {
		/*
		 * Switch the order of mount structures in the mountlist, new root
		 * mount moves to the head of the list followed by /dev and the other
		 * preserved mounts then all the preexisting mounts (old rootfs + any
		 * others)
		 */
		mount_list_lock();
		for (size_t i = 0; i < countof(preserved); i++) {
			struct preserved_mount *pmi = preserved[i];
			if (pmi->pm_rootvnode == NULLVP) {
				continue;
			}

			TAILQ_REMOVE(&mountlist, pmi->pm_mount, mnt_list);
			TAILQ_INSERT_HEAD(&mountlist, pmi->pm_mount, mnt_list);
		}
		TAILQ_REMOVE(&mountlist, incoming, mnt_list);
		TAILQ_INSERT_HEAD(&mountlist, incoming, mnt_list);
		mount_list_unlock();
	}

	/*
	 * Fixups across all volumes
	 */
	vfs_iterate(0 /* flags */, mntonname_fixup_callback, NULL);
	vfs_iterate(0 /* flags */, clear_mntk_backs_root_callback, NULL);

	error = 0;

done:
	for (size_t i = 0; i < countof(preserved); i++) {
		struct preserved_mount *pmi = preserved[i];

		if (pmi->pm_rootvnode) {
			vnode_put(pmi->pm_rootvnode);
		}
		if (pmi->pm_new_covered_vp) {
			vnode_put(pmi->pm_new_covered_vp);
		}
		if (pmi->pm_old_covered_vp) {
			vnode_rele(pmi->pm_old_covered_vp);
		}
	}

	if (outgoing_vol_new_covered_vp) {
		vnode_put(outgoing_vol_new_covered_vp);
	}

	if (incoming_vol_old_covered_vp) {
		vnode_rele(incoming_vol_old_covered_vp);
	}

	if (incoming_rootvnode) {
		vnode_put(incoming_rootvnode);
	}

	printf("%s : done shuffling mount points with error: %d\n", __FUNCTION__, error);
	return error;
 * Mount the Recovery volume of a container
vfs_mount_recovery(void)
#if CONFIG_MOUNT_PREBOOTRECOVERY
	char recoverypath[] = PLATFORM_RECOVERY_VOLUME_MOUNT_POINT; /* !const because of internal casting */
	/* Mount the recovery volume */
	printf("attempting kernel mount for recovery volume... \n");
	    recoverypath, (rootvnode->v_mount), 0, 0, (KERNEL_MOUNT_RECOVERYVOL), vfs_context_kernel());
		printf("Failed to mount recovery volume (%d)\n", error);
		printf("mounted recovery volume\n");
long busyvnodes = 0;
long deadvnodes_noreuse = 0;
int32_t freeablevnodes = 0;
uint64_t allocedvnodes = 0;
uint64_t deallocedvnodes = 0;
		vnode_drop(vp);
		vnode_hold(vp);
			vnode_hold(vp);
			vnode_drop(vp);
		vnode_drop(vp);
			vnode_hold(vp);
			vnode_drop_and_unlock(vp);
			sin = zalloc_flags(specinfo_zone, Z_WAITOK | Z_ZERO);
		} else {
			bzero(sin, sizeof(struct specinfo));
					vnode_hold(vp);
		zfree(specinfo_zone, sin);

	/* Enable atomic ops on v_usecount without the vnode lock */
	os_atomic_inc(&vp->v_usecount, relaxed);
		ut = current_uthread();
	mount_t mp = vp->v_mount;

	/*
	 * A NULL mountpoint would imply it's not attached to a any filesystem.
	 * This can only happen with a vnode created by bdevvp(). We'll consider
	 * those as not unreliable as the primary use of this function is determine
	 * which vnodes are to be handed off to the async cleaner thread for
	 * reclaim.
	 */
	if (!mp || (!(mp->mnt_kern_flag & MNTK_VIRTUALDEV) && (mp->mnt_flag & MNT_LOCAL))) {

vnode_async_list_add_locked(vnode_t vp)
	if (!(vp->v_listflag & VLIST_NO_REUSE)) {
		reusablevnodes++;
	}
	if (vp->v_flag & VCANDEALLOC) {
		os_atomic_dec(&busyvnodes, relaxed);
	}
}

static void
vnode_async_list_add(vnode_t vp)
{
	vnode_list_lock();

	if (VONLIST(vp)) {
		if (!(vp->v_listflag & VLIST_ASYNC_WORK)) {
			vnode_list_remove_locked(vp);
			vnode_async_list_add_locked(vp);
		}
	} else {
		vnode_async_list_add_locked(vp);
	}
	bool no_busy_decrement = false;
#ifdef CONFIG_IOCOUNT_TRACE
	if (!(vp->v_lflag & VL_DEAD) && (vp->v_listflag & VLIST_NO_REUSE)) {
		if (!(vp->v_listflag & VLIST_ASYNC_WORK)) {
			vnode_async_list_add_locked(vp);
		}
		no_busy_decrement = true;
	} else if ((vp->v_flag & VRAGE) && !(vp->v_lflag & VL_DEAD)) {
		reusablevnodes++;
		wakeup_laundry_thread();
			if (vp->v_flag & VCANDEALLOC) {
				TAILQ_INSERT_TAIL(&vnode_dead_list, vp, v_freelist);
				if (vp->v_listflag & VLIST_NO_REUSE) {
					deadvnodes_noreuse++;
				}
			} else {
				TAILQ_INSERT_HEAD(&vnode_dead_list, vp, v_freelist);
			}
			reusablevnodes++;
			wakeup_laundry_thread();
			reusablevnodes++;
			wakeup_laundry_thread();
	if ((vp->v_flag & VCANDEALLOC) && !no_busy_decrement) {
		os_atomic_dec(&busyvnodes, relaxed);
	}
			wakeup_laundry_thread();
		if (vp->v_flag & VCANDEALLOC) {
			os_atomic_inc(&busyvnodes, relaxed);
		}
	int32_t old_usecount;

		vnode_hold(vp);
	/* Enable atomic ops on v_usecount without the vnode lock */
	old_usecount = os_atomic_dec_orig(&vp->v_usecount, relaxed);
	if (old_usecount < 1) {
		/*
		 * Because we allow atomic ops on usecount (in lookup only, under
		 * specific conditions of already having a usecount) it is
		 * possible that when the vnode is examined, its usecount is
		 * different than what will be printed in this panic message.
		 */
		panic("vnode_rele_ext: vp %p usecount -ve : %d.  v_tag = %d, v_type = %d, v_flag = %x.",
		    vp, old_usecount - 1, vp->v_tag, vp->v_type, vp->v_flag);
#ifdef CONFIG_IOCOUNT_TRACE

	if (UBCINFOEXISTS(vp)) {
		ubc_cs_free_and_vnode_unlock(vp);
	} else {
		vnode_unlock(vp);
	}

		ut = current_uthread();
		vnode_drop_and_unlock(vp);
			vnode_hold(vp);
#ifdef CONFIG_IOCOUNT_TRACE
			vnode_drop_and_unlock(vp);
				vnode_hold(vp);
#ifdef CONFIG_IOCOUNT_TRACE
				vnode_drop_and_unlock(vp);
				vnode_hold(vp);
				vp->v_lflag |= VL_OPSCHANGE;
				vnode_drop_and_unlock(vp);
				wakeup(&vp->v_lflag); /* chkvnlock is waitng for VL_DEAD to get unset */
#if CONFIG_MACF
	if (vp->v_mount) {
		/*
		 * It is possible for bdevvp vnodes to not have a mount
		 * pointer. It's fine to let it get reclaimed without
		 * notifying.
		 */
		mac_vnode_notify_reclaim(vp);
	}
#endif

		} else {
#if CONFIG_IO_COMPRESSION_STATS
	if ((vp->io_compression_stats)) {
		vnode_iocs_record_and_free(vp);
	}
#endif /* CONFIG_IO_COMPRESSION_STATS */

	vp->v_lflag |= VL_DEAD;
				vnode_hold(vq);
					vq = vnode_drop(vq);
				vq = vnode_drop_and_unlock(vq);
	vnode_hold(vp);
	vnode_drop_and_unlock(vp);
			zfree(specinfo_zone, tmp);
		vnode_hold(vp);
			vnode_drop(vp);
		vnode_drop(vp);
	if (vq) {
		vid = vq->v_id;
		vnode_hold(vq);
	} else {
		vid = 0;
	}
			vnode_drop(vq);
		vnode_drop(vq);
				vnode_hold(vq);
				vnode_drop_and_unlock(vq);
		if (vnext) {
			vid = vnext->v_id;
			vnode_hold(vnext);
		} else {
			vid = 0;
		}
static int
vn_getpath_flags_to_buildpath_flags(int flags)
{
	int bpflags = (flags & VN_GETPATH_FSENTER) ? 0 : BUILDPATH_NO_FS_ENTER;

	if (flags && (flags != VN_GETPATH_FSENTER)) {
		if (flags & VN_GETPATH_NO_FIRMLINK) {
			bpflags |= BUILDPATH_NO_FIRMLINK;
		}
		if (flags & VN_GETPATH_VOLUME_RELATIVE) {
			bpflags |= (BUILDPATH_VOLUME_RELATIVE |
			    BUILDPATH_NO_FIRMLINK);
		}
		if (flags & VN_GETPATH_NO_PROCROOT) {
			bpflags |= BUILDPATH_NO_PROCROOT;
		}
		if (flags & VN_GETPATH_CHECK_MOVED) {
			bpflags |= BUILDPATH_CHECK_MOVED;
		}
	}

	return bpflags;
}

int
vn_getpath_ext_with_mntlen(struct vnode *vp, struct vnode *dvp, char *pathbuf,
    size_t *len, size_t *mntlen, int flags)
{
	int bpflags = vn_getpath_flags_to_buildpath_flags(flags);
	int local_len;
	int error;

	if (*len > INT_MAX) {
		return EINVAL;
	}

	local_len = *len;

	error = build_path_with_parent(vp, dvp, pathbuf, local_len, &local_len,
	    mntlen, bpflags, vfs_context_current());

	if (local_len >= 0 && local_len <= (int)*len) {
		*len = (size_t)local_len;
	}

	return error;
}

int
vn_getpath_ext(struct vnode *vp, struct vnode *dvp, char *pathbuf, size_t *len,
    int flags)
{
	return vn_getpath_ext_with_mntlen(vp, dvp, pathbuf, len, NULL, flags);
}

/*
 * Wrapper around vn_getpath_ext() that takes care of the int * <-> size_t *
 * conversion for the legacy KPIs.
 */
static int
vn_getpath_ext_int(struct vnode *vp, struct vnode *dvp, char *pathbuf,
    int *len, int flags)
{
	size_t slen = *len;
	int error;

	if (*len < 0) {
		return EINVAL;
	}

	error = vn_getpath_ext(vp, dvp, pathbuf, &slen, flags);

	if (slen <= INT_MAX) {
		*len = (int)slen;
	}

	return error;
}
	return vn_getpath_ext_int(vp, NULL, pathbuf, len, 0);
	return vn_getpath_ext_int(vp, NULL, pathbuf, len, VN_GETPATH_FSENTER);
	return vn_getpath_ext_int(vp, NULLVP, pathbuf, len,
	           VN_GETPATH_NO_FIRMLINK);
	return (int)(strlen((const char *)a) - strlen((const char *)b));
extern lck_mtx_t pkg_extensions_lck;
	int old_nentries = 0, old_maxwidth = 0;
	new_exts = kalloc_data((nentries * maxwidth) + 1, Z_WAITOK);
		kfree_data(new_exts, (nentries * maxwidth) + 1);
	lck_mtx_lock(&pkg_extensions_lck);
	old_nentries    = nexts;
	old_maxwidth    = max_ext_width;
	lck_mtx_unlock(&pkg_extensions_lck);
	kfree_data(old_exts, (old_nentries * old_maxwidth) + 1);
	int i;
	size_t extlen;
	// if the name is less than 3 bytes it can't be of the
	// form A.B and if it begins with a "." then it is also
	// not a package.
	if (len <= 3 || name[0] == '.') {
	lck_mtx_lock(&pkg_extensions_lck);
			lck_mtx_unlock(&pkg_extensions_lck);
	lck_mtx_unlock(&pkg_extensions_lck);
	if (pathlen < 0) {
		return EINVAL;
	}

		if (is_package_name(ptr, (int)(end - ptr))) {
	if (len < 0) {
		return EINVAL;
	}

		error = EPERM;
	} else {
		error = (vfsp->vfc_vfsops->vfs_sysctl)(name, namelen,
		    req->oldptr, &req->oldlen, req->newptr, req->newlen,
		    vfs_context_current());
	int     u_count; // Total volumes iterated
	int     u_only_non_system;
	uip->u_count++;
	mntname = zalloc_flags(ZV_NAMEI, Z_WAITOK | Z_NOFAIL);
	strlcpy(mntname, mp->mnt_vfsstat.f_mntonname, MAXPATHLEN);
	if (uip->u_only_non_system
	    && ((mp->mnt_flag & MNT_ROOTFS) || (mp->mnt_kern_flag & MNTK_SYSTEM))) { //MNTK_BACKS_ROOT
		printf("unmount(%d) %s skipped\n", uip->u_only_non_system, mntname);
		mount_iterdrop(mp);     // VFS_ITERATE_CB_DROPREF
	} else {
		printf("unmount(%d) %s\n", uip->u_only_non_system, mntname);

		mount_ref(mp, 0);
		mount_iterdrop(mp);     // VFS_ITERATE_CB_DROPREF
		error = dounmount(mp, MNT_FORCE, 1, vfs_context_current());
		if (error) {
			uip->u_errs++;
			printf("Unmount of %s failed (%d)\n", mntname ? mntname:"?", error);
			if (error == EBUSY) {
				uip->u_busy++;
			}
	zfree(ZV_NAMEI, mntname);
vfs_unmountall(int only_non_system)
	/*
	 * Ensure last-completion-time is valid before anyone can see that
	 * VFS shutdown has started.
	 */
	vfs_shutdown_last_completion_time = mach_absolute_time();
	OSMemoryBarrier();
	printf("vfs_unmountall(%ssystem) start\n", only_non_system ? "non" : "");
	ui.u_errs = ui.u_busy = ui.u_count = 0;
	ui.u_only_non_system = only_non_system;
	// avoid vfs_iterate deadlock in dounmount(), use VFS_ITERATE_CB_DROPREF
		goto out;
	} else if (ui.u_count < mounts) {

out:
	printf("vfs_unmountall(%ssystem) end\n", only_non_system ? "non" : "");

	/*
	 * reboot_kernel() calls us twice; once to deal with non-system
	 * mounts, and again to sweep up anything left after terminating
	 * DEXTs.  We're only finished once we've completed the second pass.
	 */
	if (!only_non_system) {
		vfs_unmountall_finished = 1;
	}
}

/*
 * vfs_shutdown_in_progress --
 *
 * Returns whether or not the VFS is shutting down the file systems.
 */
boolean_t
vfs_shutdown_in_progress(void)
{
	return vfs_unmountall_started && !vfs_unmountall_finished;
}

/*
 * vfs_shutdown_finished --
 *
 * Returns whether or not the VFS shutdown has completed.
 */
boolean_t
vfs_shutdown_finished(void)
{
	return !!vfs_unmountall_finished;
}

/*
 * vfs_update_last_completion_time --
 *
 * Updates the "last I/O completion time" timestamp used by the watchdog
 * to monitor VFS shutdown progress.  Called by various I/O stack layers
 * as operations complete and progress moves forward.
 */
void
vfs_update_last_completion_time(void)
{
	if (vfs_unmountall_started) {
		vfs_shutdown_last_completion_time = mach_absolute_time();
	}
}

/*
 * vfs_last_completion_time --
 *
 * Returns the "last I/O completion time" timestamp.  Return
 * value is a mach_absolute_time() value, and is not meaningful
 * unless vfs_is_shutting_down() also returns true.
 */
uint64_t
vfs_last_completion_time(void)
{
	return vfs_unmountall_started ? vfs_shutdown_last_completion_time : 0;
	boolean_t cs_present = FALSE;
		mp->mnt_maxreadcnt = (readmaxcnt > UINT32_MAX) ? UINT32_MAX :(uint32_t) readmaxcnt;
		mp->mnt_maxwritecnt = (writemaxcnt > UINT32_MAX) ? UINT32_MAX : (uint32_t)writemaxcnt;
	mp->mnt_alignmentmask = (uint32_t)temp;
	mp->mnt_ioqueue_depth = (uint32_t)temp;
static LCK_GRP_DECLARE(fs_klist_lck_grp, "fs_klist");
static LCK_MTX_DECLARE(fs_klist_lock, &fs_klist_lck_grp);
	lck_mtx_lock(&fs_klist_lock);
	lck_mtx_unlock(&fs_klist_lock);
		if (actual < count) {
sysctl_vfs_getvfslist(fsid_t *fsidlst, unsigned long count, unsigned long *actual)
	unsigned long actual;
	int error;
	fsidlst = kalloc_data(req->oldlen, Z_WAITOK | Z_ZERO);
		kfree_data(fsidlst, req->oldlen);
	kfree_data(fsidlst, req->oldlen);
	struct mount *mp = NULL;
	int *name, namelen;
	int flags = 0;
	union {
		struct statfs64 sfs64;
		struct user64_statfs osfs64;
		struct user32_statfs osfs32;
	} *sfsbuf;

	if (req->newptr == USER_ADDR_NULL) {
		error = EINVAL;
		goto out;
	}
#if CONFIG_MACF
		error = mac_mount_check_umount(ctx, mp);
		if (error != 0) {
			goto out;
		}
#endif
	case VFS_CTL_OSTATFS:
	case VFS_CTL_STATFS64:

		sfsbuf = kalloc_type(typeof(*sfsbuf), Z_WAITOK);

		if (name[0] == VFS_CTL_STATFS64) {
			struct statfs64 *sfs = &sfsbuf->sfs64;

			vfs_get_statfs64(mp, sfs);
			error = SYSCTL_OUT(req, sfs, sizeof(*sfs));
		} else if (is_64_bit) {
			struct user64_statfs *sfs = &sfsbuf->osfs64;

			bzero(sfs, sizeof(*sfs));
			sfs->f_flags = mp->mnt_flag & MNT_VISFLAGMASK;
			sfs->f_type = (short)mp->mnt_vtable->vfc_typenum;
			sfs->f_bsize = (user64_long_t)sp->f_bsize;
			sfs->f_iosize = (user64_long_t)sp->f_iosize;
			sfs->f_blocks = (user64_long_t)sp->f_blocks;
			sfs->f_bfree = (user64_long_t)sp->f_bfree;
			sfs->f_bavail = (user64_long_t)sp->f_bavail;
			sfs->f_files = (user64_long_t)sp->f_files;
			sfs->f_ffree = (user64_long_t)sp->f_ffree;
			sfs->f_fsid = sp->f_fsid;
			sfs->f_owner = sp->f_owner;
				strlcpy(&sfs->f_fstypename[0], &mp->fstypename_override[0], MFSNAMELEN);
			} else {
				strlcpy(sfs->f_fstypename, sp->f_fstypename, MFSNAMELEN);
			strlcpy(sfs->f_mntonname, sp->f_mntonname, MNAMELEN);
			strlcpy(sfs->f_mntfromname, sp->f_mntfromname, MNAMELEN);
			error = SYSCTL_OUT(req, sfs, sizeof(*sfs));
			struct user32_statfs *sfs = &sfsbuf->osfs32;
			long temp;

			bzero(sfs, sizeof(*sfs));
			sfs->f_flags = mp->mnt_flag & MNT_VISFLAGMASK;
			sfs->f_type = (short)mp->mnt_vtable->vfc_typenum;
				sfs->f_blocks = (user32_long_t)__SHIFT_OR_CLIP(sp->f_blocks, shift);
				sfs->f_bfree = (user32_long_t)__SHIFT_OR_CLIP(sp->f_bfree, shift);
				sfs->f_bavail = (user32_long_t)__SHIFT_OR_CLIP(sp->f_bavail, shift);
				sfs->f_bsize = (user32_long_t)(sp->f_bsize << shift);
				temp = lmax(sp->f_iosize, sp->f_bsize);
				if (temp > INT32_MAX) {
					error = EINVAL;
					kfree_type(typeof(*sfsbuf), sfsbuf);
					goto out;
				}
				sfs->f_iosize = (user32_long_t)temp;
				sfs->f_bsize = (user32_long_t)sp->f_bsize;
				sfs->f_iosize = (user32_long_t)sp->f_iosize;
				sfs->f_blocks = (user32_long_t)sp->f_blocks;
				sfs->f_bfree = (user32_long_t)sp->f_bfree;
				sfs->f_bavail = (user32_long_t)sp->f_bavail;
			}
			sfs->f_files = (user32_long_t)sp->f_files;
			sfs->f_ffree = (user32_long_t)sp->f_ffree;
			sfs->f_fsid = sp->f_fsid;
			sfs->f_owner = sp->f_owner;

				strlcpy(&sfs->f_fstypename[0], &mp->fstypename_override[0], MFSNAMELEN);
			} else {
				strlcpy(sfs->f_fstypename, sp->f_fstypename, MFSNAMELEN);
			strlcpy(sfs->f_mntonname, sp->f_mntonname, MNAMELEN);
			strlcpy(sfs->f_mntfromname, sp->f_mntfromname, MNAMELEN);
			error = SYSCTL_OUT(req, sfs, sizeof(*sfs));
		kfree_type(typeof(*sfsbuf), sfsbuf);
	lck_mtx_lock(&fs_klist_lock);
	lck_mtx_unlock(&fs_klist_lock);
	lck_mtx_lock(&fs_klist_lock);
	lck_mtx_unlock(&fs_klist_lock);
	lck_mtx_lock(&fs_klist_lock);
	lck_mtx_unlock(&fs_klist_lock);
	lck_mtx_lock(&fs_klist_lock);
	lck_mtx_unlock(&fs_klist_lock);
SYSCTL_EXTENSIBLE_NODE(_vfs, VFS_GENERIC, generic,
    CTLFLAG_RW | CTLFLAG_LOCKED, NULL, "vfs generic hinge");
process_vp(vnode_t vp, int want_vp, bool can_defer, int *deferred)
	vnode_hold(vp);
		vnode_drop_and_unlock(vp);
#ifdef CONFIG_IOCOUNT_TRACE
		vnode_drop_and_unlock(vp);
		if ((want_vp || can_defer) && (vnode_on_reliable_media(vp) == FALSE || (vp->v_flag & VISDIRTY))) {
			vnode_drop_and_unlock(vp);
			vnode_drop_and_unlock(vp);
		vp = process_vp(vp, 0, false, &deferred);
__attribute__((noreturn))
static void
vn_laundry_continue(void)
	struct freelst *free_q;
	struct ragelst *rage_q;
	bool rage_q_empty;
	bool free_q_empty;
	free_q = &vnode_free_list;
	rage_q = &vnode_rage_list;
	for (;;) {
		vnode_list_lock();
		if (!TAILQ_EMPTY(&vnode_async_work_list)) {
			vp = TAILQ_FIRST(&vnode_async_work_list);
			async_work_handled++;
			vp = process_vp(vp, 0, false, &deferred);

			if (vp != NULLVP) {
				panic("found VBAD vp (%p) on async queue", vp);
			}
			continue;
		}

		free_q_empty = TAILQ_EMPTY(free_q);
		rage_q_empty = TAILQ_EMPTY(rage_q);

		if (!rage_q_empty && !free_q_empty) {
			struct timeval current_tv;

			microuptime(&current_tv);
			if (ragevnodes < rage_limit &&
			    ((current_tv.tv_sec - rage_tv.tv_sec) < RAGE_TIME_LIMIT)) {
				rage_q_empty = true;
			}
		}

		if (numvnodes < numvnodes_min || (rage_q_empty && free_q_empty) ||
		    (reusablevnodes <= reusablevnodes_max && deadvnodes >= deadvnodes_high)) {
			assert_wait(free_q, (THREAD_UNINT));

			vnode_list_unlock();

			thread_block((thread_continue_t)vn_laundry_continue);

			continue;
		}

		if (!rage_q_empty) {
			vp = TAILQ_FIRST(rage_q);
		} else {
			vp = TAILQ_FIRST(free_q);
		}

		vp = process_vp(vp, 0, false, &deferred);

		if (vp != NULLVP) {
			/* If process_vp returns a vnode, it is locked and has a holdcount */
			vnode_drop_and_unlock(vp);
			vp = NULLVP;
		}
	}
}

static inline void
wakeup_laundry_thread()
{
	if (deadvnodes_noreuse || (numvnodes >= numvnodes_min && deadvnodes < deadvnodes_low &&
	    (reusablevnodes > reusablevnodes_max || numvnodes >= desiredvnodes))) {
		wakeup(&vnode_free_list);
	}
}

/*
 * This must be called under vnode_list_lock() to prevent race when accessing
 * various vnode stats.
 */
static void
send_freeable_vnodes_telemetry(void)
{
	bool send_event = false;

	/*
	 * Log an event when the 'numvnodes' is above the freeable vnodes threshold
	 * or when it falls back within the threshold.
	 * When the 'numvnodes' is above the threshold, log an event when it has
	 * been incrementally growing by 25%.
	 */
	if ((numvnodes > desiredvnodes) && (freevnodes + deadvnodes) == 0) {
		long last_numvnodes = freeable_vnodes_telemetry.numvnodes;

		if (numvnodes > (last_numvnodes + ((last_numvnodes * 25) / 100)) ||
		    numvnodes >= numvnodes_max) {
			send_event = true;
		}
		freeablevnodes_threshold_crossed = true;
	} else if (freeablevnodes_threshold_crossed &&
	    (freevnodes + deadvnodes) > busyvnodes) {
		freeablevnodes_threshold_crossed = false;
		send_event = true;
	}

	if (__improbable(send_event)) {
		ca_event_t event = CA_EVENT_ALLOCATE_FLAGS(freeable_vnodes, Z_NOWAIT);

		if (event) {
			/*
			 * Update the stats except the 'numvnodes_max' and 'desiredvnodes'
			 * as they are immutable after init.
			 */
			freeable_vnodes_telemetry.numvnodes_min = numvnodes_min;
			freeable_vnodes_telemetry.numvnodes = numvnodes;
			freeable_vnodes_telemetry.freevnodes = freevnodes;
			freeable_vnodes_telemetry.deadvnodes = deadvnodes;
			freeable_vnodes_telemetry.freeablevnodes = freeablevnodes;
			freeable_vnodes_telemetry.busyvnodes = busyvnodes;
			freeable_vnodes_telemetry.threshold_crossed =
			    freeablevnodes_threshold_crossed;

			memcpy(event->data, &freeable_vnodes_telemetry,
			    sizeof(CA_EVENT_TYPE(freeable_vnodes)));

			if (!freeablevnodes_threshold_crossed) {
				freeable_vnodes_telemetry.numvnodes = 0;
			}
			CA_EVENT_SEND(event);
		}
	}
}

static int
new_vnode(vnode_t *vpp, bool can_free)
{
	long force_alloc_min;
	vnode_t vp;
	uint32_t retries = 0, max_retries = 100;                /* retry incase of tablefull */
	uint32_t bdevvp_vnodes = 0;
	int force_alloc = 0, walk_count = 0;
	boolean_t need_reliable_vp = FALSE;
	int deferred;
	struct timeval initial_tv;
	struct timeval current_tv;
	proc_t  curproc = current_proc();
	bool force_alloc_freeable = false;

	if (vn_dealloc_level == DEALLOC_VNODE_NONE) {
		can_free = false;
	}

	initial_tv.tv_sec = 0;
retry:
	vp = NULLVP;

	vnode_list_lock();
	newvnode++;

	if (need_reliable_vp == TRUE) {
		async_work_timed_out++;
	}

	/*
	 * The vnode list lock was dropped after force_alloc_freeable was set,
	 * reevaluate.
	 */
	force_alloc_min = MAX(desiredvnodes, numvnodes_min);
	if (force_alloc_freeable &&
	    (numvnodes < force_alloc_min || numvnodes >= numvnodes_max)) {
		force_alloc_freeable = false;
	}

	if (((numvnodes - deadvnodes + deadvnodes_noreuse) < desiredvnodes) ||
	    force_alloc || force_alloc_freeable) {
		struct timespec ts;
		uint32_t vflag = 0;

		/*
		 * Can always reuse a dead one except if it is in the process of
		 * being freed or the FS cannot handle freeable vnodes.
		 */
		if (!TAILQ_EMPTY(&vnode_dead_list)) {
			/* Select an appropriate deadvnode */
			if (numvnodes <= numvnodes_min || !can_free) {
				/* all vnodes upto numvnodes_min are not freeable */
				vp = TAILQ_FIRST(&vnode_dead_list);
				if (numvnodes > numvnodes_min &&
				    (vp->v_flag & VCANDEALLOC)) {
					/*
					 * Freeable vnodes are added to the
					 * back of the queue, so if the first
					 * from the front is freeable, then
					 * there are none on the dead list.
					 */
					vp = NULLVP;
				}
			} else {
				/*
				 * Filesystems which opt in to freeable vnodes
				 * can get either one.
				 */
				TAILQ_FOREACH_REVERSE(vp, &vnode_dead_list,
				    deadlst, v_freelist) {
					if (!(vp->v_listflag & VLIST_NO_REUSE)) {
						break;
					}
				}
			}

			if (vp) {
				force_alloc_freeable = false;
				goto steal_this_vp;
			}
		}

		/*
		if (force_alloc) {
			numvnodes_min++;
		} else if (can_free && (numvnodes > numvnodes_min)) {
			allocedvnodes++;
			freeablevnodes++;
			vflag = VCANDEALLOC;

			send_freeable_vnodes_telemetry();
		}
		if (nc_smr_enabled) {
			vp = zalloc_smr(vnode_zone, Z_WAITOK_ZERO_NOFAIL);
		} else {
			vp = zalloc_flags(vnode_zone, Z_WAITOK_ZERO_NOFAIL);
		}

		lck_mtx_init(&vp->v_lock, &vnode_lck_grp, &vnode_lck_attr);
		vp->v_id = (uint32_t)ts.tv_nsec;
		vp->v_flag = VSTANDARD | vflag;
		if (force_alloc_freeable) {
			/* This vnode should be recycled and freed immediately */
			vp->v_lflag = VL_MARKTERM;
			vp->v_listflag = VLIST_NO_REUSE;
		}

		if (vflag & VCANDEALLOC) {
			os_atomic_inc(&busyvnodes, relaxed);
		}
#if CONFIG_IOCOUNT_TRACE
		if (__improbable(bootarg_vnode_iocount_trace)) {
			vp->v_iocount_trace = (vnode_iocount_trace_t)zalloc_permanent(
				IOCOUNT_TRACE_MAX_TYPES * sizeof(struct vnode_iocount_trace),
				ZALIGN(struct vnode_iocount_trace));
		}
#endif /* CONFIG_IOCOUNT_TRACE */

#if CONFIG_FILE_LEASES
		LIST_INIT(&vp->v_leases);
#endif



			if (((curproc->p_flag & P_DEPENDENCY_CAPABLE) == 0 || vp->v_mount == NULL ||
			    (vp->v_mount->mnt_kern_flag & MNTK_VIRTUALDEV) == 0) &&
			    !(vp->v_listflag & VLIST_NO_REUSE) &&
			    (can_free || !(vp->v_flag & VCANDEALLOC))) {
			if (((curproc->p_flag & P_DEPENDENCY_CAPABLE) == 0 || vp->v_mount == NULL ||
			    (vp->v_mount->mnt_kern_flag & MNTK_VIRTUALDEV) == 0) &&
			    !(vp->v_listflag & VLIST_NO_REUSE) &&
			    (can_free || !(vp->v_flag & VCANDEALLOC))) {
		if (can_free && (vn_dealloc_level > DEALLOC_VNODE_NONE) &&
		    (numvnodes >= force_alloc_min) && (numvnodes < numvnodes_max)) {
			force_alloc_freeable = true;
			vnode_list_unlock();
			goto retry;
		}

		log(LOG_EMERG, "%d desired, %ld numvnodes, "
		    "%ld free, %ld dead, %ld async, %d rage %d bdevvp\n",
		    desiredvnodes, numvnodes, freevnodes, deadvnodes, async_work_vnodes, ragevnodes, bdevvp_vnodes);
			panic("vnode table is full");
			panic("vnode table is full");
	newvnode_nodead++;
	if ((vp = process_vp(vp, 1, true, &deferred)) == NULLVP) {
			elapsed_msecs = (int)(elapsed_tv.tv_sec * 1000 + elapsed_tv.tv_usec / 1000);
	if (vp->v_lflag & VL_LABELED || mac_vnode_label(vp) != NULL) {
	if (can_free && (vp->v_flag & VCANDEALLOC)) {
		vp->v_flag = VSTANDARD | VCANDEALLOC;
	} else {
		vp->v_flag = VSTANDARD;
	}

	/* process_vp returns a locked vnode with a holdcount */
	vnode_drop_and_unlock(vp);
void
vnode_hold(vnode_t vp)
{
	int32_t old_holdcount = os_atomic_inc_orig(&vp->v_holdcount, relaxed);

	if (old_holdcount == INT32_MAX) {
		/*
		 * Because we allow atomic ops on the holdcount it is
		 * possible that when the vnode is examined, its holdcount
		 * is different than what will be printed in this
		 * panic message.
		 */
		panic("%s: vp %p holdcount overflow from : %d v_tag = %d, v_type = %d, v_flag = %x.",
		    __FUNCTION__, vp, old_holdcount, vp->v_tag, vp->v_type, vp->v_flag);
	}
}

#define VNODE_HOLD_NO_SMR    (1<<29) /* Disable vnode_hold_smr */

/*
 * To be used when smr is the only protection (cache_lookup and cache_lookup_path)
 */
bool
vnode_hold_smr(vnode_t vp)
{
	int32_t holdcount;

	/*
	 * For "high traffic" vnodes like rootvnode, the atomic
	 * cmpexcg loop below can turn into a infinite loop, no need
	 * to do it for vnodes that won't be dealloc'ed
	 */
	if (!(os_atomic_load(&vp->v_flag, relaxed) & VCANDEALLOC)) {
		vnode_hold(vp);
		return true;
	}

	for (;;) {
		holdcount = os_atomic_load(&vp->v_holdcount, relaxed);

		if (holdcount & VNODE_HOLD_NO_SMR) {
			return false;
		}

		if ((os_atomic_cmpxchg(&vp->v_holdcount, holdcount, holdcount + 1, relaxed) != 0)) {
			return true;
		}
	}
}

/*
 * free callback from smr enabled zones
 */
static void
vnode_smr_free(void *_vp, __unused size_t _size)
{
	vnode_t vp = _vp;

	bzero(vp, sizeof(*vp));
}

static vnode_t
vnode_drop_internal(vnode_t vp, bool locked)
{
	int32_t old_holdcount = os_atomic_dec_orig(&vp->v_holdcount, relaxed);

	if (old_holdcount < 1) {
		if (locked) {
			vnode_unlock(vp);
		}

		/*
		 * Because we allow atomic ops on the holdcount it is possible
		 * that when the vnode is examined, its holdcount is different
		 * than what will be printed in this panic message.
		 */
		panic("%s : vp %p holdcount -ve: %d.  v_tag = %d, v_type = %d, v_flag = %x.",
		    __FUNCTION__, vp, old_holdcount - 1, vp->v_tag, vp->v_type, vp->v_flag);
	}

	if (vn_dealloc_level == DEALLOC_VNODE_NONE || old_holdcount > 1 ||
	    !(vp->v_flag & VCANDEALLOC) || !(vp->v_lflag & VL_DEAD)) {
		if (locked) {
			vnode_unlock(vp);
		}
		return vp;
	}

	if (!locked) {
		vnode_lock(vp);
	}

	if ((os_atomic_load(&vp->v_holdcount, relaxed) != 0) || vp->v_iocount ||
	    vp->v_usecount || !(vp->v_flag & VCANDEALLOC) || !(vp->v_lflag & VL_DEAD)) {
		vnode_unlock(vp);
		return vp;
	}

	vnode_list_lock();

	/*
	 * the v_listflag field is protected by the vnode_list_lock
	 */
	if (VONLIST(vp) && (vp->v_listflag & VLIST_DEAD) &&
	    (numvnodes > desiredvnodes || (vp->v_listflag & VLIST_NO_REUSE) ||
	    vn_dealloc_level != DEALLOC_VNODE_ALL || deadvnodes >= deadvnodes_high) &&
	    (os_atomic_cmpxchg(&vp->v_holdcount, 0, VNODE_HOLD_NO_SMR, relaxed) != 0)) {
		VREMDEAD("vnode_list_remove", vp);
		numvnodes--;
		freeablevnodes--;
		deallocedvnodes++;
		vp->v_listflag = 0;

		send_freeable_vnodes_telemetry();
		vnode_list_unlock();

#if CONFIG_MACF
		struct label *tmpl = mac_vnode_label(vp);
		vp->v_label = NULL;
#endif /* CONFIG_MACF */

		vnode_unlock(vp);

#if CONFIG_MACF
		if (tmpl) {
			mac_vnode_label_free(tmpl);
		}
#endif /* CONFIG_MACF */

		if (nc_smr_enabled) {
			zfree_smr(vnode_zone, vp);
		} else {
			zfree(vnode_zone, vp);
		}

		vp = NULLVP;
	} else {
		vnode_list_unlock();
		vnode_unlock(vp);
	}

	return vp;
}

vnode_t
vnode_drop_and_unlock(vnode_t vp)
{
	return vnode_drop_internal(vp, true);
}

vnode_t
vnode_drop(vnode_t vp)
{
	return vnode_drop_internal(vp, false);
}

SYSCTL_NODE(_vfs, OID_AUTO, vnstats, CTLFLAG_RD | CTLFLAG_LOCKED, NULL, "vfs vnode stats");
SYSCTL_COMPAT_INT(_vfs_vnstats, OID_AUTO, vn_dealloc_level,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &vn_dealloc_level, 0, "");
SYSCTL_COMPAT_INT(_vfs_vnstats, OID_AUTO, desired_vnodes,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &desiredvnodes, 0, "");
SYSCTL_LONG(_vfs_vnstats, OID_AUTO, num_vnodes,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &numvnodes, "");
SYSCTL_COMPAT_INT(_vfs_vnstats, OID_AUTO, num_vnodes_min,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &numvnodes_min, 0, "");
SYSCTL_COMPAT_INT(_vfs_vnstats, OID_AUTO, num_vnodes_max,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &numvnodes_max, 0, "");
SYSCTL_COMPAT_INT(_vfs_vnstats, OID_AUTO, num_deallocable_vnodes,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &freeablevnodes, 0, "");
SYSCTL_LONG(_vfs_vnstats, OID_AUTO, num_deallocable_busy_vnodes,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &busyvnodes, "");
SYSCTL_LONG(_vfs_vnstats, OID_AUTO, num_dead_vnodes,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &deadvnodes, "");
SYSCTL_LONG(_vfs_vnstats, OID_AUTO, num_dead_vnodes_to_dealloc,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &deadvnodes_noreuse, "");
SYSCTL_LONG(_vfs_vnstats, OID_AUTO, num_async_work_vnodes,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &async_work_vnodes, "");
SYSCTL_COMPAT_INT(_vfs_vnstats, OID_AUTO, num_rapid_aging_vnodes,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &ragevnodes, 0, "");
SYSCTL_LONG(_vfs_vnstats, OID_AUTO, num_free_vnodes,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &freevnodes, "");
SYSCTL_LONG(_vfs_vnstats, OID_AUTO, num_recycledvnodes,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &num_recycledvnodes, "");
SYSCTL_QUAD(_vfs_vnstats, OID_AUTO, num_allocedvnodes,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &allocedvnodes, "");
SYSCTL_QUAD(_vfs_vnstats, OID_AUTO, num_deallocedvnodes,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &deallocedvnodes, "");
SYSCTL_QUAD(_vfs_vnstats, OID_AUTO, num_newvnode_calls,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &newvnode, "");
SYSCTL_QUAD(_vfs_vnstats, OID_AUTO, num_newvnode_calls_nodead,
    CTLFLAG_RD | CTLFLAG_LOCKED,
    &newvnode_nodead, "");
#ifdef CONFIG_IOCOUNT_TRACE
__private_extern__ int
vnode_getalways_from_pager(vnode_t vp)
	return vget_internal(vp, 0, VNODE_ALWAYS | VNODE_PAGER);
static int
vnode_put_internal_locked(vnode_t vp, bool from_pager)

	if (((vp->v_lflag & (VL_DEAD | VL_NEEDINACTIVE)) == VL_NEEDINACTIVE)) {

		if (UBCINFOEXISTS(vp)) {
			ubc_cs_free_and_vnode_unlock(vp);
		} else {
			vnode_unlock(vp);
		}
		if (from_pager) {
			/*
			 * We can't initiate reclaim when called from the pager
			 * because it will deadlock with itself so we hand it
			 * off to the async cleaner thread.
			 */
			vnode_async_list_add(vp);
		} else {
			vnode_lock_convert(vp);
			vnode_reclaim_internal(vp, 1, 1, 0);
		}
int
vnode_put_locked(vnode_t vp)
{
	return vnode_put_internal_locked(vp, false);
}

int
vnode_put(vnode_t vp)
{
	int retval;

	vnode_lock_spin(vp);
	vnode_hold(vp);
	retval = vnode_put_internal_locked(vp, false);
	vnode_drop_and_unlock(vp);

	return retval;
}

int
vnode_put_from_pager(vnode_t vp)
{
	int retval;

	vnode_lock_spin(vp);
	vnode_hold(vp);
	/* Cannot initiate reclaim while paging */
	retval = vnode_put_internal_locked(vp, true);
	vnode_drop_and_unlock(vp);

	return retval;
}

int
vnode_writecount(vnode_t vp)
{
	return vp->v_writecount;
}

/* is vnode_t in use by others?  */
int
kauth_cred_t
vnode_cred(vnode_t vp)
{
	if (vp->v_cred) {
		return kauth_cred_require(vp->v_cred);
	}

	return NULL;
}

	int forpager = vflags & VNODE_PAGER;
	if (!forpager && (++vp->v_references >= UNAGE_THRESHHOLD ||
	    (vp->v_flag & VISDIRTY && vp->v_references >= UNAGE_DIRTYTHRESHHOLD))) {
#ifdef CONFIG_IOCOUNT_TRACE
#ifdef CONFIG_IOCOUNT_TRACE
	bool clear_tty_revoke = false;
	/*
	 * We have to force any terminals in reads to return and give up
	 * their iocounts. It's important to do this after VL_TERMINATE
	 * has been set to ensure new reads are blocked while the
	 * revoke is in progress.
	 */
	if (vnode_istty(vp) && (flags & REVOKEALL) && (vp->v_iocount > 1)) {
		clear_tty_revoke = true;
	if (clear_tty_revoke) {
		vnode_unlock(vp);
		VNOP_IOCTL(vp, TIOCREVOKECLEAR, (caddr_t)NULL, 0, vfs_context_kernel());
		vnode_lock(vp);
	}

#if CONFIG_FILE_LEASES
	/*
	 * Revoke all leases in place for this vnode as it is about to be reclaimed.
	 * In normal case, there shouldn't be any leases in place by the time we
	 * get here as there shouldn't be any opens on the vnode (usecount == 0).
	 * However, in the case of force unmount or unmount of a volume that
	 * contains file that was opened with O_EVTONLY then the vnode can be
	 * reclaimed while the file is still opened.
	 */
	vnode_revokelease(vp, true);
#endif

		kfree_type(struct fifoinfo, fip);
#if CONFIG_FILE_LEASES
	if (__improbable(!LIST_EMPTY(&vp->v_leases))) {
		panic("vnode_reclaim_internal: vleases NOT empty");
	}
#endif

#if CONFIG_IOCOUNT_TRACE
	if (__improbable(bootarg_vnode_iocount_trace)) {
		bzero(vp->v_iocount_trace,
		    IOCOUNT_TRACE_MAX_TYPES * sizeof(struct vnode_iocount_trace));
	}
#endif /* CONFIG_IOCOUNT_TRACE */

    vnode_create_options_t vc_options)
	vnode_t vp = NULLVP;
	bool existing_vnode;
	bool init_vnode = !(vc_options & VNODE_CREATE_EMPTY);

		existing_vnode = true;
		existing_vnode = false;
		if ((error = new_vnode(&vp, !(vc_options & VNODE_CREATE_NODEALLOC)))) {
			vnode_lock(vp);
			vnode_unlock(vp);
		vnode_lock(vp);
		vnode_unlock(vp);
	vp->v_type = (uint16_t)param->vnfs_vtype;
#ifdef CONFIG_IOCOUNT_TRACE
			vnode_hold(vp);
			vnode_lock(vp);
			vnode_put_locked(vp);
			vnode_drop_and_unlock(vp);
#ifdef CONFIG_IOCOUNT_TRACE
#ifdef CONFIG_IOCOUNT_TRACE
			vnode_hold(vp);
			vnode_lock(vp);
#ifdef CONFIG_IOCOUNT_TRACE
			vnode_put_locked(vp);
			vnode_drop_and_unlock(vp);
			bool is_bdevvp;

			is_bdevvp = (vp->v_flag & VBDEVVP);

			if (is_bdevvp) {
				printf("%s: alias vnode %p is in state of change (start)\n",
				    __func__, vp);
			}

			vnode_hold(vp);
			vp->v_lflag |= VL_OPSCHANGE;
			vp->v_type = (uint16_t)param->vnfs_vtype;
			vp->v_lflag = VL_OPSCHANGE;

			if (is_bdevvp) {
				printf("%s: alias vnode %p is in state of change (end)\n",
				    __func__, vp);
			}

			vnode_drop_and_unlock(vp);
			wakeup(&vp->v_lflag); /* chkvnlock is waitng for VL_DEAD to get unset */
		fip = kalloc_type(struct fifoinfo, Z_WAITOK | Z_ZERO);
				panic("insmntque: vp on the free list");
	ut = current_uthread();
	case SECLUDED_FILECACHE_NONE:
	case SECLUDED_FILECACHE_APPS:
	case SECLUDED_FILECACHE_RDONLY:
int
vnode_create_ext(uint32_t flavor, uint32_t size, void *data, vnode_t *vpp, vnode_create_options_t vc_options)
{
	if (vc_options & ~(VNODE_CREATE_EMPTY | VNODE_CREATE_NODEALLOC)) {
		return EINVAL;
	}
	*vpp = NULLVP;
	return vnode_create_internal(flavor, size, data, vpp, vc_options);
}

	return vnode_create_ext(flavor, size, data, vpp, VNODE_CREATE_NODEALLOC);
	return vnode_create_ext(VNCREATE_FLAVOR, VCREATESIZE, NULL,
	           vpp, VNODE_CREATE_EMPTY);
vnode_initialize(uint32_t __unused flavor, uint32_t size, void *data, vnode_t *vpp)
	return vnode_create_internal(flavor, size, data, vpp, VNODE_CREATE_DEFAULT);
		panic("addfsref: vp on the free list");
	fsid_list = kalloc_data(count * sizeof(fsid_t), Z_WAITOK);
	kfree_data(allocmem, count * sizeof(fsid_t));
	if (get_system_inshutdown() != 0) {
	struct nameidata *ndp;
	int error = 0;
	ndp = kalloc_type(struct nameidata, Z_WAITOK | Z_NOFAIL);

	NDINIT(ndp, LOOKUP, OP_LOOKUP, ndflags, UIO_SYSSPACE,
		ndp->ni_dvp = start_dvp;
		ndp->ni_cnd.cn_flags |= USEDVP;
	if ((error = namei(ndp))) {
		goto out_free;
	ndp->ni_cnd.cn_flags &= ~USEDVP;
	*vpp = ndp->ni_vp;
	nameidone(ndp);
out_free:
	kfree_type(struct nameidata, ndp);
	return error;
	struct nameidata *ndp = NULL;
	ndp = kalloc_type(struct nameidata, Z_WAITOK | Z_NOFAIL);

	NDINIT(ndp, LOOKUP, OP_OPEN, ndflags, UIO_SYSSPACE,
	if ((error = vn_open(ndp, fmode, cmode))) {
		*vpp = ndp->ni_vp;
	kfree_type(struct nameidata, ndp);
			/*
			 * Only free the ACL buffer if 'VA_FILESEC_ACL' is not set as it
			 * should be freed by the caller or it is a post-inheritance copy.
			 */
			if (!(vap->va_vaflags & VA_FILESEC_ACL) ||
			    (oacl != NULL && nacl != oacl)) {
				kauth_acl_free(nacl);
			}
	if (vnode_isdir(vp)) {
		if ((fmode & (FWRITE | O_TRUNC)) || /* disallow write operations on directories */
		    ((fmode & FSEARCH) && !(fmode & O_DIRECTORY))) {
			return EISDIR;
		}
	} else {
		if (fmode & O_DIRECTORY) {
			return ENOTDIR;
		}
		if (vp->v_type == VSOCK && vp->v_tag != VT_FDESC) {
			return EOPNOTSUPP;    /* Operation not supported on socket */
		}
		if (vp->v_type == VLNK && (fmode & O_NOFOLLOW) != 0) {
			return ELOOP;         /* O_NOFOLLOW was specified and the target is a symbolic link */
		}
		if (cnp->cn_ndp->ni_flag & NAMEI_TRAILINGSLASH) {

		if (!vnode_isreg(vp) && (fmode & FEXEC)) {
			return EACCES;
		}
	if (fmode & (FSEARCH | FEXEC)) {
		if (vnode_isdir(vp)) {
			action |= KAUTH_VNODE_SEARCH;
		} else {
			action |= KAUTH_VNODE_EXECUTE;
		}
	}
	/*
	 * Processes with the appropriate entitlement can marked themselves as
	 * ignoring file/directory permissions if they own it.
	 */
	if (!owner_ok && proc_ignores_node_permissions(vfs_context_proc(vcp->ctx))) {
		owner_ok = 1;
	}


			if (vauth_file_owner(vcp) && proc_ignores_node_permissions(vfs_context_proc(vcp->ctx))) {
				KAUTH_DEBUG("%p    Override DENY due to entitlement", vcp->vp);
				return 0;
			}
			if (vauth_dir_owner(vcp) && proc_ignores_node_permissions(vfs_context_proc(vcp->ctx))) {
				KAUTH_DEBUG("%p    Override DENY due to entitlement", vcp->vp);
				return 0;
			}
			if (vauth_file_owner(vcp) && proc_ignores_node_permissions(vfs_context_proc(vcp->ctx))) {
				KAUTH_DEBUG("%p    Override DENY due to entitlement", vcp->vp);
				return 0;
			}
vnode_authorize_checkimmutable(mount_t mp, vauth_ctx vcp,
    struct vnode_attr *vap, int rights, int ignore)
			if ((rights & (KAUTH_VNODE_ADD_FILE | KAUTH_VNODE_ADD_SUBDIRECTORY | KAUTH_VNODE_WRITE_EXTATTRIBUTES | ~KAUTH_VNODE_WRITE_RIGHTS)) == rights) {
			if ((rights & (KAUTH_VNODE_APPEND_DATA | KAUTH_VNODE_WRITE_EXTATTRIBUTES | ~KAUTH_VNODE_WRITE_RIGHTS)) == rights) {
			if (error && !ignore) {
				/*
				 * In case of a rename, we want to check ownership for dvp as well.
				 */
				int owner = 0;
				if (rights & KAUTH_VNODE_DELETE_CHILD && vcp->dvp != NULL) {
					owner = vauth_file_owner(vcp) && vauth_dir_owner(vcp);
				} else {
					owner = vauth_file_owner(vcp);
				}
				if (owner && proc_ignores_node_permissions(vfs_context_proc(vcp->ctx))) {
					error = vnode_immutable(vap, append, 1);
				}
			}
		}
		if (error) {
	int             parent_action = 0; /* In case we need to use namedstream's data fork for cached rights*/
	if ((result = vnode_authorize_checkimmutable(mp, vcp, vcp->vap, rights,
		result = vnode_authorize_checkimmutable(mp, vcp, vcp->dvap,
#define VNODE_SEC_ATTRS_NO_ACL (VNODE_ATTR_va_uid | VNODE_ATTR_va_gid | VNODE_ATTR_va_mode | VNODE_ATTR_va_flags | VNODE_ATTR_va_type)

	uid_t ouid = vap->va_uid;
	gid_t ogid = vap->va_gid;
	}

	if (mp) {
		if (vfs_extendedsecurity(mp) && VATTR_IS_ACTIVE(vap, va_acl) && !VATTR_IS_SUPPORTED(vap, va_acl)) {
			panic("(1) vnode attrs not complete for vnode_attr_authorize");
		vnode_attr_handle_uid_and_gid(vap, mp, ctx);
	}

	if ((vap->va_active & VNODE_SEC_ATTRS_NO_ACL) != (vap->va_supported & VNODE_SEC_ATTRS_NO_ACL)) {
		panic("(2) vnode attrs not complete for vnode_attr_authorize (2) vap->va_active = 0x%llx , vap->va_supported = 0x%llx",
		    vap->va_active, vap->va_supported);
	if (mp) {
		vap->va_uid = ouid;
		vap->va_gid = ogid;
	}

		VATTR_SET(vap, va_mode, ACCESSPERMS & ~vfs_context_proc(ctx)->p_fd.fd_cmask);
		KAUTH_DEBUG("ATTR - defaulting new file mode to %o from umask %o",
		    vap->va_mode, vfs_context_proc(ctx)->p_fd.fd_cmask);
	int eofflag, siz = UIO_BUFF_SIZE, alloc_size = 0, nentries = 0;
	uio_stackbuf_t uio_buf[UIO_SIZEOF(1)];
	uthread_t ut = current_uthread();
	rbuf = kalloc_data(siz, Z_WAITOK);
	alloc_size = siz;
	kfree_data(rbuf, alloc_size);
#ifdef CONFIG_IOCOUNT_TRACE
record_iocount_trace_vnode(vnode_t vp, int type)
	void *stacks[IOCOUNT_TRACE_MAX_FRAMES] = {0};
	int idx = vp->v_iocount_trace[type].idx;
	if (idx >= IOCOUNT_TRACE_MAX_IDX) {

	OSBacktrace((void **)&stacks[0], IOCOUNT_TRACE_MAX_FRAMES);

	/*
	 * To save index space, only store the unique backtraces. If dup is found,
	 * just bump the count and return.
	 */
	for (int i = 0; i < idx; i++) {
		if (memcmp(&stacks[0], &vp->v_iocount_trace[type].stacks[i][0],
		    sizeof(stacks)) == 0) {
			vp->v_iocount_trace[type].counts[i]++;
			return;
		}
	memcpy(&vp->v_iocount_trace[type].stacks[idx][0], &stacks[0],
	    sizeof(stacks));
	vp->v_iocount_trace[type].counts[idx] = 1;
	vp->v_iocount_trace[type].idx++;
}

static void
record_iocount_trace_uthread(vnode_t vp, int count)
{
	struct uthread *ut;

	ut = current_uthread();
static void
record_vp(vnode_t vp, int count)
{
	if (__probable(bootarg_vnode_iocount_trace == 0 &&
	    bootarg_uthread_iocount_trace == 0)) {
		return;
	}
	if (vp->v_resolve) {
		return;
	}
#endif
	if ((vp->v_flag & VSYSTEM)) {
		return;
	}
	if (bootarg_vnode_iocount_trace) {
		record_iocount_trace_vnode(vp,
		    (count > 0) ? IOCOUNT_TRACE_VGET : IOCOUNT_TRACE_VPUT);
	}
	if (bootarg_uthread_iocount_trace) {
		record_iocount_trace_uthread(vp, count);
	}
}
#endif /* CONFIG_IOCOUNT_TRACE */
#if CONFIG_TRIGGERS
#define __triggers_unused
#define __triggers_unused       __unused
vfs_resolver_result(__triggers_unused uint32_t seq, __triggers_unused enum resolver_status stat, __triggers_unused int aux)
#if CONFIG_TRIGGERS
#else
	return (0x0ULL) | (((uint64_t)ENOTSUP) << 4) | (((uint64_t)RESOLVER_ERROR) & 0xF);
#endif
#if CONFIG_TRIGGERS

#define TRIG_DEBUG 0

#if TRIG_DEBUG
#define TRIG_LOG(...) do { printf("%s: ", __FUNCTION__); printf(__VA_ARGS__); } while (0)
#else
#define TRIG_LOG(...)
#endif

/*
 * Resolver result functions
 */


	rp = kalloc_type(struct vnode_resolve, Z_WAITOK | Z_NOFAIL);
	lck_mtx_init(&rp->vr_lock, &trigger_vnode_lck_grp, &trigger_vnode_lck_attr);
	kfree_type(struct vnode_resolve, rp);
	lck_mtx_destroy(&rp->vr_lock, &trigger_vnode_lck_grp);
	kfree_type(struct vnode_resolve, rp);
	/*
	 * N.B. we cannot call vfs_context_can_resolve_triggers()
	 * here because we really only want to suppress that in
	 * the event the trigger will be resolved by something in
	 * user-space.  Any triggers that are resolved by the kernel
	 * do not pose a threat of deadlock.
	 */

		vnode_drop(infop->trigger_vp);
	} else if (infop->trigger_vp != NULLVP) {
		vnode_drop(infop->trigger_vp);

			vnode_hold(infop->trigger_vp);
		vnode_drop(vp);
	} else if (info.trigger_vp != NULLVP) {
		vnode_drop(info.trigger_vp);
	struct nameidata *ndp;
	ndp = kalloc_type(struct nameidata, Z_WAITOK | Z_NOFAIL);

	NDINIT(ndp, LOOKUP, OP_LOOKUP, USEDVP | NOCROSSMOUNT | FOLLOW, UIO_SYSSPACE,
	ndp->ni_dvp = rvp;
	res = namei(ndp);
	vp = ndp->ni_vp;
	nameidone(ndp);
	kfree_type(struct nameidata, ndp);
#define NPATH_WORDS (MAXPATHLEN / sizeof(unsigned long))
	/*
	 * Must be a multiple of 4, then -1, for tracing!
	 */
	unsigned long path[NPATH_WORDS + (4 - (NPATH_WORDS % 4)) - 1];
vnode_trace_path_callback(struct vnode *vp, void *vctx)
	struct vnode_trace_paths_context *ctx = vctx;
	size_t path_len = sizeof(ctx->path);
	int getpath_len = (int)path_len;
	if (vn_getpath(vp, (char *)ctx->path, &getpath_len) == 0) {
		/* vn_getpath() NUL-terminates, and len includes the NUL. */
		assert(getpath_len >= 0);
		path_len = (size_t)getpath_len;
		assert(path_len <= sizeof(ctx->path));
		kdebug_vfs_lookup(ctx->path, (int)path_len, vp,

#if CONFIG_FILE_LEASES
#include <IOKit/IOBSD.h>
#include <sys/file_internal.h>

#define FILE_LEASES_ENTITLEMENT    "com.apple.private.vfs.file-leases"

static uint32_t lease_break_timeout = 60; /* secs */

#if (DEVELOPMENT || DEBUG)
static int lease_debug = 0;
static int lease_entitlement_override = 0;

SYSCTL_NODE(_vfs, OID_AUTO, lease, CTLFLAG_RW | CTLFLAG_LOCKED, NULL, "vfs lease");
SYSCTL_UINT(_vfs_lease, OID_AUTO, break_timeout, CTLFLAG_RW | CTLFLAG_LOCKED, &lease_break_timeout, 0, "");
SYSCTL_INT(_vfs_lease, OID_AUTO, debug, CTLFLAG_RW | CTLFLAG_LOCKED, &lease_debug, 0, "");
SYSCTL_INT(_vfs_lease, OID_AUTO, entitlement_override, CTLFLAG_RW | CTLFLAG_LOCKED, &lease_entitlement_override, 0, "");

#define LEASEDBG(fmt, args...)                                       \
do {                                                                 \
	if (__improbable(lease_debug)) {                                 \
	        pid_t cur_pid = proc_getpid(current_proc());             \
	        printf("%s(%d): " fmt "\n", __func__, cur_pid, ##args);  \
	}                                                                \
} while(0)
#else
#define LEASEDBG(fmt, args...)  /**/
#endif /* (DEVELOPMENT || DEBUG) */

static bool
allow_setlease(vfs_context_t ctx)
{
	bool entitled;

	entitled = IOTaskHasEntitlement(vfs_context_task(ctx),
	    FILE_LEASES_ENTITLEMENT);

#if (DEVELOPMENT || DEBUG)
	if (!entitled) {
		entitled = (lease_entitlement_override == 1);
	}
#endif

	return entitled;
}

static file_lease_t
file_lease_alloc(struct fileglob *fg, int fl_type, pid_t pid)
{
	file_lease_t fl;

	fl = kalloc_type(struct file_lease, Z_WAITOK);
	/*
	 * Duplicated file descriptors created by dup() or fork() would have the
	 * same 'fileglob' so the lease can be released or modified with the
	 * duplicated fds. Opening the same file (by either same or different
	 * process) would have different 'fileglob' so a lease always follows a
	 * 'fileglob'.
	 */
	fl->fl_fg = fg;
	fl->fl_type = fl_type;
	fl->fl_pid = pid;
	fl->fl_downgrade_start = fl->fl_release_start = 0;

	return fl;
}

static void
file_lease_free(file_lease_t fl)
{
	kfree_type(struct file_lease, fl);
}

/*
 * A read lease can be placed only on a file/directory that is opened for
 * read-only which means no other processes have the file/directory opened in
 * read-write/write-only mode or mmap'ed writable.
 * A write lease can be placed on a file only if there are no other opens
 * for the file.
 *
 * Needs to be called with vnode's lock held.
 */
static int
check_for_open_conflict(vnode_t vp, struct fileglob *fg, int fl_type,
    int expcounts)
{
	int error = 0;

	if (fl_type == F_RDLCK) {
		if (vp->v_writecount > expcounts &&
		    !(vp->v_writecount == 1 && (fg->fg_flag & FWRITE))) {
			error = EAGAIN;
		} else if (ubc_is_mapped_writable(vp)) {
			error = EAGAIN;
		}
	} else if (fl_type == F_WRLCK && vp->v_usecount > expcounts) {
		error = EAGAIN;
	}

	return error;
}

/* Needs to be called with vnode's lock held. */
static void
modify_file_lease(vnode_t vp, file_lease_t fl, int new_fl_type,
    struct fileglob *new_fg)
{
	LEASEDBG("fl %p changing fl_type from %d to %d (flags 0x%x)",
	    fl, fl->fl_type, new_fl_type, fl->fl_flags);

	fl->fl_type = new_fl_type;

	/*
	 * The lease being modified may be using a different file
	 * descriptor, so usurp the fileglob pointer here.  In this
	 * case the old descriptor no longer holds the lease.
	 */
	if (new_fg != NULL) {
		fl->fl_fg = new_fg;
	}

	if (fl->fl_flags & FL_FLAG_RELEASE_PENDING ||
	    fl->fl_flags & FL_FLAG_DOWNGRADE_PENDING) {
		wakeup(&vp->v_leases);
	}
}

static int
acquire_file_lease(vnode_t vp, struct fileglob *fg, int fl_type, int expcounts,
    vfs_context_t ctx)
{
	file_lease_t fl, new_fl, our_fl;
	int error;

	/* Make sure "expected count" looks sane. */
	if (expcounts < 0 || expcounts > OPEN_MAX) {
		return EINVAL;
	}

	new_fl = file_lease_alloc(fg, fl_type, vfs_context_pid(ctx));

	vnode_lock(vp);

	error = check_for_open_conflict(vp, fg, fl_type, expcounts);
	if (error) {
		LEASEDBG("open conflict on vp %p type %d writecnt %d usecnt %d "
		    "fl_type %d expcounts %d",
		    vp, vp->v_type, vp->v_writecount, vp->v_usecount, fl_type,
		    expcounts);
		goto out;
	}

	our_fl = NULL;
	LIST_FOREACH(fl, &vp->v_leases, fl_link) {
		/* Does the existing lease belong to us? */
		if (fl->fl_fg == new_fl->fl_fg ||
		    fl->fl_pid == new_fl->fl_pid) {
			our_fl = fl;
			continue;
		}

		/*
		 * We don't allow placing a new write lease when there is an existing
		 * read lease that doesn't belong to us. We also don't allow putting
		 * a new read lease if there is a pending release on the lease.
		 * Putting a new read lease when there is a pending downgrade on the
		 * lease is fine as it won't cause lease conflict.
		 */
		if (fl_type == F_WRLCK || fl->fl_flags & FL_FLAG_RELEASE_PENDING) {
			break;
		}
	}

	/*
	 * Found an existing lease that we don't own and it conflicts with the
	 * new lease.
	 */
	if (fl) {
		LEASEDBG("lease conflict on vp %p fl %p fl_type %d cur_fl_type %d",
		    vp, fl, fl_type, fl->fl_type);
		goto out;
	}

	/* Found an existing lease that we own so just change the type. */
	if (our_fl) {
		LEASEDBG("replace lease on vp %p fl %p old_fl_type %d new_fl_type %d",
		    vp, our_fl, our_fl->fl_type, fl_type);

		modify_file_lease(vp, our_fl, new_fl->fl_type, new_fl->fl_fg);
		goto out;
	}

	LEASEDBG("acquired lease on vp %p type %d fl %p fl_type %d fg %p",
	    vp, vp->v_type, new_fl, new_fl->fl_type, new_fl->fl_fg);

	LIST_INSERT_HEAD(&vp->v_leases, new_fl, fl_link);
	new_fl = NULL;

out:
	vnode_unlock(vp);

	if (new_fl) {
		file_lease_free(new_fl);
	}

	return error;
}

static int
release_file_lease(vnode_t vp, struct fileglob *fg)
{
	file_lease_t fl, fl_tmp;
	int error = 0;

	LEASEDBG("request to release lease on vp %p type %d fg %p",
	    vp, vp->v_type, fg);

	vnode_lock(vp);

	LIST_FOREACH_SAFE(fl, &vp->v_leases, fl_link, fl_tmp) {
		if (fl->fl_fg == fg) {
			LEASEDBG("released lease on vp %p fl %p type %d",
			    vp, fl, fl->fl_type);

			LIST_REMOVE(fl, fl_link);
			modify_file_lease(vp, fl, F_UNLCK, NULL);
			break;
		}
	}

	vnode_unlock(vp);

	if (fl) {
		file_lease_free(fl);
	} else {
		error = ENOLCK;
	}

	return error;
}

/*
 * Acquire or release a file lease according to the given type (F_RDLCK,
 * F_WRLCK or F_UNLCK).
 *
 * Returns:	0			Success
 *		EAGAIN			Failed to acquire a file lease due to conflicting opens
 *		ENOLCK			Failed to release a file lease due to lease not found
 *		EPERM           Current task doesn't have the entitlement
 */
int
vnode_setlease(vnode_t vp, struct fileglob *fg, int fl_type, int expcounts,
    vfs_context_t ctx)
{
	int error;

	if (!allow_setlease(ctx)) {
		return EPERM;
	}

	error = (fl_type == F_UNLCK) ? release_file_lease(vp, fg) :
	    acquire_file_lease(vp, fg, fl_type, expcounts, ctx);

	return error;
}

/*
 * Retrieve the currently in place lease for the file.
 *
 * Returns:
 *		F_RDLCK			Read lease
 *		F_WRLCK			Write lease
 *		F_UNLCK			No lease
 */
int
vnode_getlease(vnode_t vp)
{
	file_lease_t fl;
	int fl_type = F_UNLCK;

	vnode_lock(vp);

	/*
	 * There should be only one type of lease in the list as read and write
	 * leases can't co-exist for the same file.
	 */
	fl = LIST_FIRST(&vp->v_leases);
	if (fl) {
		fl_type = fl->fl_type;
	}

	vnode_unlock(vp);

	LEASEDBG("vp %p fl %p fl_type %d", vp, fl, fl_type);

	return fl_type;
}

/* Must be called with vnode's lock held. */
static bool
check_for_lease_conflict(vnode_t vp, int breaker_fl_type, vfs_context_t ctx)
{
	file_lease_t fl;
	pid_t pid = vfs_context_pid(ctx);
	bool is_conflict = false;

	LIST_FOREACH(fl, &vp->v_leases, fl_link) {
		if ((fl->fl_type == F_WRLCK && fl->fl_pid != pid) ||
		    (breaker_fl_type == F_WRLCK && fl->fl_pid != pid)) {
			LEASEDBG("conflict detected on vp %p type %d fl_type %d "
			    "breaker_fl_type %d",
			    vp, vp->v_type, fl->fl_type, breaker_fl_type);

			is_conflict = true;
			break;
		}
	}

	return is_conflict;
}

static uint64_t
absolutetime_elapsed_in_secs(uint64_t start)
{
	uint64_t elapsed, elapsed_sec;
	uint64_t now = mach_absolute_time();

	elapsed = now - start;
	absolutetime_to_nanoseconds(elapsed, &elapsed_sec);
	elapsed_sec /= NSEC_PER_SEC;

	return elapsed_sec;
}

/* Must be called with vnode's lock held. */
static void
handle_lease_break_timedout(vnode_t vp)
{
	file_lease_t fl, fl_tmp;
	uint64_t elapsed_sec;

	LIST_FOREACH_SAFE(fl, &vp->v_leases, fl_link, fl_tmp) {
		if (fl->fl_flags & FL_FLAG_DOWNGRADE_PENDING) {
			elapsed_sec = absolutetime_elapsed_in_secs(fl->fl_downgrade_start);

			if (elapsed_sec >= lease_break_timeout) {
				LEASEDBG("force downgrade on vp %p for fl %p elapsed %llu "
				    "timeout %u", vp, fl, elapsed_sec, lease_break_timeout);

				fl->fl_flags &= ~FL_FLAG_DOWNGRADE_PENDING;
				fl->fl_downgrade_start = 0;
				modify_file_lease(vp, fl, F_RDLCK, NULL);
				continue;
			}
		}
		if (fl->fl_flags & FL_FLAG_RELEASE_PENDING) {
			elapsed_sec = absolutetime_elapsed_in_secs(fl->fl_release_start);

			if (elapsed_sec >= lease_break_timeout) {
				LEASEDBG("force release on vp %p for fl %p elapsed %llu "
				    "timeout %u", vp, fl, elapsed_sec, lease_break_timeout);

				LIST_REMOVE(fl, fl_link);
				file_lease_free(fl);
				continue;
			}
		}
	}

	/* Wakeup the lease breaker(s). */
	wakeup(&vp->v_leases);
}

/* Must be called with vnode's lock held. */
static void
wait_for_lease_break(vnode_t vp, int breaker_fl_type, vfs_context_t ctx)
{
	file_lease_t fl;
	struct timespec ts;
	uint64_t elapsed_sec, start_time;
	int error;

restart:
	fl = LIST_FIRST(&vp->v_leases);
	assert(fl);

	/*
	 * In a rare case it is possible that the lease that we are blocked on has
	 * been released and a new lease has been put in place after we are
	 * signalled to wake up. In this particular, we would treat it as no
	 * conflict and proceed. This could only happen for directory leasing.
	 */
	if ((fl->fl_flags & (FL_FLAG_DOWNGRADE_PENDING | FL_FLAG_RELEASE_PENDING)) == 0) {
		LEASEDBG("new lease in place on vp %p fl %p fl_type %d "
		    "breaker_fl_type %d",
		    vp, fl, fl->fl_type, breaker_fl_type);

		return;
	}
	/*
	 * Figure out which timer to use for lease break timedout as we could have
	 * both timers active. If both timers active, pick the one with earliest
	 * start time.
	 */
	if (fl->fl_release_start) {
		if (fl->fl_downgrade_start == 0 ||
		    fl->fl_downgrade_start < fl->fl_release_start) {
			start_time = fl->fl_release_start;
		} else {
			start_time = fl->fl_downgrade_start;
		}
	} else {
		start_time = fl->fl_downgrade_start;
	}
	assert(start_time > 0);

	elapsed_sec = absolutetime_elapsed_in_secs(start_time);

	LEASEDBG("elapsed_sec %llu release_start %llu downgrade_start %llu",
	    elapsed_sec, fl->fl_release_start, fl->fl_downgrade_start);

	ts.tv_sec = (lease_break_timeout > elapsed_sec ?
	    (lease_break_timeout - elapsed_sec) : 0);
	ts.tv_nsec = (ts.tv_sec == 0 ? 1 : 0);
	error = msleep(&vp->v_leases, &vp->v_lock, PVFS, __func__, &ts);

	if (error == 0 || error != EWOULDBLOCK) {
		/*
		 * Woken up due to lease is released/downgraded by lease holder.
		 * We don't expect any other error from msleep() beside EWOULDBLOCK.
		 * Check if there is any further conflicts. If so, then continue to
		 * wait for the next conflict to resolve.
		 */
		if (check_for_lease_conflict(vp, breaker_fl_type, ctx)) {
			goto restart;
		}
	} else {
		/*
		 * Woken due to lease break timeout expired (EWOULDBLOCK returned).
		 * Break/downgrade all conflicting leases.
		 */
		handle_lease_break_timedout(vp);

		if (check_for_lease_conflict(vp, breaker_fl_type, ctx)) {
			goto restart;
		}
	}
}

/* Must be called with vnode's lock held. */
static void
send_lease_break_event(vnode_t vp, uint32_t event)
{
	if (vp->v_knotes.slh_first != NULL) {
		KNOTE(&vp->v_knotes, event);
	}
}

/*
 * Break lease(s) in place for the file when there is conflict.
 * This function would return 0 for almost all call sites. The only exception
 * is when it is called from open1() with O_NONBLOCK flag and it needs to block
 * waiting for the lease conflict(s) to resolve. In this case EWOULDBLOCK is
 * returned.
 */
int
vnode_breaklease(vnode_t vp, uint32_t oflags, vfs_context_t ctx)
{
	file_lease_t fl;
	uint64_t now;
	int fl_type;
	int error = 0;

	vnode_lock(vp);

	if (__probable(LIST_EMPTY(&vp->v_leases))) {
		goto out_unlock;
	}

	/* Determine the access mode requested by the lease breaker. */
	fl_type = (oflags & (O_WRONLY | O_RDWR | O_CREAT | O_TRUNC)) ? F_WRLCK : F_RDLCK;

	/*
	 * If the lease-breaker is just reading, check that it can break
	 * leases first.  If the lease-breaker is writing, or if the
	 * context was not specified, we always break.
	 */
	if (fl_type == F_RDLCK && !vfs_context_can_break_leases(ctx)) {
		goto out_unlock;
	}

	if (!check_for_lease_conflict(vp, fl_type, ctx)) {
		goto out_unlock;
	}

	now = mach_absolute_time();

	LEASEDBG("break lease on vp %p type %d oflags 0x%x cur_time %llu",
	    vp, vp->v_type, oflags, now);

	/*
	 * We get to this point then this means all lease(s) are conflict and
	 * we need to send the lease break event to the lease holder(s).
	 * It is possible that a lease could have both downgrade and release events
	 * pending triggered by multiple breakers trying to open the file in
	 * different modes. Both events would have different lease break timers.
	 * Consider the following case:
	 * 1. Process A holds the write lease on file X.
	 * 2. Provess B opens the file X in read-only mode.
	 *    This triggers downgrade lease event to Process A.
	 * 3. While downgrade is pending, Process C opens the file X in read-write
	 *    mode. This triggers release lease event to Process A.
	 */
	LIST_FOREACH(fl, &vp->v_leases, fl_link) {
		if (fl_type == F_WRLCK) {
			/* File is opened for writing or truncate. */
			if (fl->fl_flags & FL_FLAG_RELEASE_PENDING) {
				continue;
			}
			fl->fl_release_start = now;
			fl->fl_flags |= FL_FLAG_RELEASE_PENDING;
			send_lease_break_event(vp, NOTE_LEASE_RELEASE);
		} else {
			/* File is opened for reading. */
			if (fl->fl_flags & FL_FLAG_DOWNGRADE_PENDING ||
			    fl->fl_flags & FL_FLAG_RELEASE_PENDING) {
				continue;
			}
			fl->fl_downgrade_start = now;
			fl->fl_flags |= FL_FLAG_DOWNGRADE_PENDING;
			send_lease_break_event(vp, NOTE_LEASE_DOWNGRADE);
		}
	}

	/*
	 * If open is requested with O_NONBLOCK, then we can't block and wait for
	 * the lease to be released/downgraded. Just bail out with EWOULDBLOCK.
	 */
	if (oflags & O_NONBLOCK) {
		error = EWOULDBLOCK;
		goto out_unlock;
	}

	wait_for_lease_break(vp, fl_type, ctx);

	LEASEDBG("break lease on vp %p oflags 0x%x, error %d", vp, oflags, error);

out_unlock:
	vnode_unlock(vp);

	return error;
}

/*
 * Get parent vnode by parent ID (only for file system that supports
 * MNTK_PATH_FROM_ID).
 * On success, the parent's vnode is returned with iocount held.
 */
static vnode_t
vnode_getparent_byid(vnode_t vp)
{
	struct vnode_attr va;
	vnode_t dvp = NULLVP;
	vfs_context_t ctx = vfs_context_current();
	int error;

	if (!(vp->v_mount->mnt_kern_flag & MNTK_PATH_FROM_ID)) {
		goto out;
	}

	VATTR_INIT(&va);
	VATTR_WANTED(&va, va_parentid);

	/* Get the vnode's parent id from the file system. */
	error = vnode_getattr(vp, &va, ctx);
	if (error || !VATTR_IS_SUPPORTED(&va, va_parentid)) {
		goto out;
	}

	/*
	 * Ask the file system for the parent vnode.
	 * We are ignoring the error here as we don't expect the parent vnode to be
	 * populated on error.
	 */
	(void)VFS_VGET(vp->v_mount, (ino64_t)va.va_parentid, &dvp, ctx);

out:
	return dvp;
}

/*
 * Break directory's lease.
 * If 'need_parent' is true, then parent is obtained via vnode_getparent() (or
 * vnode_getparent_byid()) on the provided 'vp'.
 */
void
vnode_breakdirlease(vnode_t vp, bool need_parent, uint32_t oflags)
{
	vnode_t dvp;

	if ((vnode_vtype(vp) != VREG && vnode_vtype(vp) != VDIR) ||
	    (vp == rootvnode)) {
		return;
	}

	/*
	 * If parent is not provided, first try to get it from the name cache.
	 * If failed, then we will attempt to ask the file system for parent vnode.
	 * This is just a best effort as both attempts could still fail.
	 */
	if (need_parent) {
		dvp = vnode_getparent(vp);
		if (__improbable(dvp == NULLVP)) {
			dvp = vnode_getparent_byid(vp);
		}
	} else {
		dvp = vp;
	}

	if (__probable(dvp != NULLVP)) {
		/* Always break dir leases. */
		(void)vnode_breaklease(dvp, oflags, NULL);
	}

	if (need_parent && (dvp != NULLVP)) {
		vnode_put(dvp);
	}
}

/*
 * Revoke all lease(s) in place for the file.
 * This is called when the vnode is reclaimed.
 */
void
vnode_revokelease(vnode_t vp, bool locked)
{
	file_lease_t fl, fl_tmp;
	bool need_wakeup = false;

	if ((vnode_vtype(vp) != VREG && vnode_vtype(vp) != VDIR)) {
		return;
	}

	if (!locked) {
		vnode_lock(vp);
	}

	LIST_FOREACH_SAFE(fl, &vp->v_leases, fl_link, fl_tmp) {
		LIST_REMOVE(fl, fl_link);
		file_lease_free(fl);
		need_wakeup = true;
	}

	/* Wakeup any lease breaker(s) that might be currently blocked. */
	if (__improbable(need_wakeup)) {
		wakeup(&vp->v_leases);
	}

	if (!locked) {
		vnode_unlock(vp);
	}
}

#endif /* CONFIG_FILE_LEASES */