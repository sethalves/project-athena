//
//  Image.h
//  image/src/image
//
//  Created by Clement Brisset on 4/5/2017.
//  Copyright 2017 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_image_Image_h
#define hifi_image_Image_h

#include <QVariant>

#include <gpu/Texture.h>

class QByteArray;
class QImage;

namespace image {

namespace TextureUsage {

enum Type {
    DEFAULT_TEXTURE,
    STRICT_TEXTURE,
    ALBEDO_TEXTURE,
    NORMAL_TEXTURE,
    BUMP_TEXTURE,
    SPECULAR_TEXTURE,
    METALLIC_TEXTURE = SPECULAR_TEXTURE, // for now spec and metallic texture are the same, converted to grey
    ROUGHNESS_TEXTURE,
    GLOSS_TEXTURE,
    EMISSIVE_TEXTURE,
    CUBE_TEXTURE,
    OCCLUSION_TEXTURE,
    SCATTERING_TEXTURE = OCCLUSION_TEXTURE,
    LIGHTMAP_TEXTURE,
    UNUSED_TEXTURE
};

using TextureLoader = std::function<gpu::TexturePointer(const QImage&, const std::string&, const std::atomic<bool>&)>;
TextureLoader getTextureLoaderForType(Type type, const QVariantMap& options = QVariantMap());

gpu::TexturePointer create2DTextureFromImage(const QImage& image, const std::string& srcImageName,
                                             const std::atomic<bool>& abortProcessing);
gpu::TexturePointer createStrict2DTextureFromImage(const QImage& image, const std::string& srcImageName,
                                                   const std::atomic<bool>& abortProcessing);
gpu::TexturePointer createAlbedoTextureFromImage(const QImage& image, const std::string& srcImageName,
                                                 const std::atomic<bool>& abortProcessing);
gpu::TexturePointer createEmissiveTextureFromImage(const QImage& image, const std::string& srcImageName,
                                                   const std::atomic<bool>& abortProcessing);
gpu::TexturePointer createNormalTextureFromNormalImage(const QImage& image, const std::string& srcImageName,
                                                       const std::atomic<bool>& abortProcessing);
gpu::TexturePointer createNormalTextureFromBumpImage(const QImage& image, const std::string& srcImageName,
                                                     const std::atomic<bool>& abortProcessing);
gpu::TexturePointer createRoughnessTextureFromImage(const QImage& image, const std::string& srcImageName,
                                                    const std::atomic<bool>& abortProcessing);
gpu::TexturePointer createRoughnessTextureFromGlossImage(const QImage& image, const std::string& srcImageName,
                                                         const std::atomic<bool>& abortProcessing);
gpu::TexturePointer createMetallicTextureFromImage(const QImage& image, const std::string& srcImageName,
                                                   const std::atomic<bool>& abortProcessing);
gpu::TexturePointer createCubeTextureFromImage(const QImage& image, const std::string& srcImageName,
                                               const std::atomic<bool>& abortProcessing);
gpu::TexturePointer createCubeTextureFromImageWithoutIrradiance(const QImage& image, const std::string& srcImageName,
                                                                const std::atomic<bool>& abortProcessing);
gpu::TexturePointer createLightmapTextureFromImage(const QImage& image, const std::string& srcImageName,
                                                   const std::atomic<bool>& abortProcessing);

gpu::TexturePointer process2DTextureColorFromImage(const QImage& srcImage, const std::string& srcImageName, bool isStrict,
                                                   const std::atomic<bool>& abortProcessing);
gpu::TexturePointer process2DTextureNormalMapFromImage(const QImage& srcImage, const std::string& srcImageName, bool isBumpMap,
                                                       const std::atomic<bool>& abortProcessing);
gpu::TexturePointer process2DTextureGrayscaleFromImage(const QImage& srcImage, const std::string& srcImageName, bool isInvertedPixels,
                                                       const std::atomic<bool>& abortProcessing);
gpu::TexturePointer processCubeTextureColorFromImage(const QImage& srcImage, const std::string& srcImageName, bool generateIrradiance,
                                                     const std::atomic<bool>& abortProcessing);

} // namespace TextureUsage

const QStringList getSupportedFormats();

bool isColorTexturesCompressionEnabled();
bool isNormalTexturesCompressionEnabled();
bool isGrayscaleTexturesCompressionEnabled();
bool isCubeTexturesCompressionEnabled();

void setColorTexturesCompressionEnabled(bool enabled);
void setNormalTexturesCompressionEnabled(bool enabled);
void setGrayscaleTexturesCompressionEnabled(bool enabled);
void setCubeTexturesCompressionEnabled(bool enabled);

gpu::TexturePointer processImage(const QByteArray& content, const std::string& url,
                                 int maxNumPixels, TextureUsage::Type textureType,
                                 const std::atomic<bool>& abortProcessing = false);

} // namespace image

#endif // hifi_image_Image_h
