document.addEventListener('DOMContentLoaded', () => {
    const statsElement = document.getElementById('stats');
    const frameImage = document.getElementById('frame') as HTMLImageElement;

    if (statsElement && frameImage) {
        frameImage.onload = () => {
            const width = frameImage.naturalWidth;
            const height = frameImage.naturalHeight;
            statsElement.innerText = `Resolution: ${width}x${height}`;
        };

        // Handle cases where the image is already cached and loaded
        if (frameImage.complete) {
            frameImage.onload(new Event('load'));
        }
    }
});
