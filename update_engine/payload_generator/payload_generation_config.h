//
// Copyright (C) 2015 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef UPDATE_ENGINE_PAYLOAD_GENERATOR_PAYLOAD_GENERATION_CONFIG_H_
#define UPDATE_ENGINE_PAYLOAD_GENERATOR_PAYLOAD_GENERATION_CONFIG_H_

#include <cstddef>

#include <memory>
#include <string>
#include <vector>

#include <brillo/key_value_store.h>

#include "update_engine/payload_consumer/payload_constants.h"
#include "update_engine/payload_generator/filesystem_interface.h"
#include "update_engine/update_metadata.pb.h"

namespace chromeos_update_engine {

struct PostInstallConfig {
  // Whether the postinstall config is empty.
  bool IsEmpty() const;

  // Whether this partition carries a filesystem with post-install program that
  // must be run to finalize the update process.
  bool run = false;

  // The path to the post-install program relative to the root of this
  // filesystem.
  std::string path;

  // The filesystem type used to mount the partition in order to run the
  // post-install program.
  std::string filesystem_type;
};

struct PartitionConfig {
  explicit PartitionConfig(std::string name) : name(name) {}

  // Returns whether the PartitionConfig is not an empty image and all the
  // fields are set correctly to a valid image file.
  bool ValidateExists() const;

  // Open then filesystem stored in this partition and stores it in
  // |fs_interface|. Returns whether opening the filesystem worked.
  bool OpenFilesystem();

  // The path to the partition file. This can be a regular file or a block
  // device such as a loop device.
  std::string path;

  // The size of the data in |path|. If rootfs verification is used (verity)
  // this value should match the size of the verity device for the rootfs, and
  // the size of the whole kernel. This value could be smaller than the
  // partition and is the size of the data update_engine assumes verified for
  // the source image, and the size of that data it should generate for the
  // target image.
  uint64_t size = 0;

  // The FilesystemInterface implementation used to access this partition's
  // files.
  std::unique_ptr<FilesystemInterface> fs_interface;

  std::string name;

  PostInstallConfig postinstall;
};

// The ImageConfig struct describes a pair of binaries kernel and rootfs and the
// metadata associated with the image they are part of, like build number, size,
// etc.
struct ImageConfig {
  // Returns whether the ImageConfig is an empty image.
  bool ValidateIsEmpty() const;

  // Load |rootfs_size| and |kernel.size| from the respective image files. For
  // the kernel, the whole |kernel.path| file is assumed. For the rootfs, the
  // size is detected from the filesystem.
  // Returns whether the image size was properly detected.
  bool LoadImageSize();

  // Load postinstall config from a key value store.
  bool LoadPostInstallConfig(const brillo::KeyValueStore& store);

  // Returns whether the |image_info| field is empty.
  bool ImageInfoIsEmpty() const;

  // The ImageInfo message defined in the update_metadata.proto file describes
  // the metadata of the image.
  ImageInfo image_info;

  // The updated partitions.
  std::vector<PartitionConfig> partitions;
};

struct PayloadVersion {
  PayloadVersion() : PayloadVersion(0, 0) {}
  PayloadVersion(uint64_t major_version, uint32_t minor_version);

  // Returns whether the PayloadVersion is valid.
  bool Validate() const;

  // Return whether the passed |operation| is allowed by this payload.
  bool OperationAllowed(InstallOperation_Type operation) const;

  // Whether this payload version is a delta payload.
  bool IsDelta() const;

  // Tells whether the update is done in-place, that is, whether the operations
  // read and write from the same partition.
  bool InplaceUpdate() const;

  // The major version of the payload.
  uint64_t major;

  // The minor version of the payload.
  uint32_t minor;

  // Wheter the IMGDIFF operation is allowed based on the available compressor
  // in the delta_generator and the one supported by the target.
  bool imgdiff_allowed = false;
};

// The PayloadGenerationConfig struct encapsulates all the configuration to
// build the requested payload. This includes information about the old and new
// image as well as the restrictions applied to the payload (like minor-version
// and full/delta payload).
struct PayloadGenerationConfig {
  // Returns whether the PayloadGenerationConfig is valid.
  bool Validate() const;

  // Image information about the new image that's the target of this payload.
  ImageConfig target;

  // Image information pertaining the old image, if any. This is only valid
  // if is_full is false, so we are requested a delta payload.
  ImageConfig source;

  // Wheter the requested payload is a delta payload.
  bool is_delta = false;

  // The major/minor version of the payload.
  PayloadVersion version;

  // The size of the rootfs partition, that not necessarily is the same as the
  // filesystem in either source or target version, since there is some space
  // after the partition used to store the verity hashes and or the bootcache.
  uint64_t rootfs_partition_size = 0;

  // The |hard_chunk_size| is the maximum size that a single operation should
  // write in the destination. Operations bigger than chunk_size should be
  // split. A value of -1 means no hard chunk size limit. A very low limit
  // means more operations, and less of a chance to reuse the data.
  ssize_t hard_chunk_size = -1;

  // The |soft_chunk_size| is the preferred chunk size to use when there's no
  // significant impact to the operations. For example, REPLACE, MOVE and
  // SOURCE_COPY operations are not significantly impacted by the chunk size,
  // except for a few bytes overhead in the manifest to describe extra
  // operations. On the other hand, splitting BSDIFF operations impacts the
  // payload size since it is not possible to use the redundancy *between*
  // chunks.
  size_t soft_chunk_size = 2 * 1024 * 1024;

  // TODO(deymo): Remove the block_size member and maybe replace it with a
  // minimum alignment size for blocks (if needed). Algorithms should be able to
  // pick the block_size they want, but for now only 4 KiB is supported.

  // The block size used for all the operations in the manifest.
  size_t block_size = 4096;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_PAYLOAD_GENERATOR_PAYLOAD_GENERATION_CONFIG_H_
