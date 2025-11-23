# Feathered Edge Plugin for CloudCompare

## Overview

The Feathered Edge plugin creates aesthetically pleasing feathered edges on point clouds using distance-based random reduction with customizable fall-off curves. This is particularly useful for point clouds from terrestrial scanning or photogrammetry that have hard edges or cutoffs.

## Features

- **Distance-based Point Reduction**: Points are probabilistically removed based on their distance from an origin point
- **Multiple Fall-off Curves**: Choose from Linear, Exponential, Gaussian, or Smoothstep fall-off types
- **Adjustable Parameters**: Control the extent and strength of the feathering effect
- **Reproducible Results**: Optional random seed for consistent results
- **Non-destructive**: Creates a new point cloud, preserving the original

## How It Works

1. User selects an origin point (cloud center or manual coordinates)
2. Plugin calculates Euclidean distance from origin to all points
3. For each point, a "keep probability" is calculated based on:
   - Distance from origin
   - Maximum distance parameter
   - Fall-off curve type
   - Strength parameter
4. Points are randomly kept or removed based on their probability
5. A new "feathered" point cloud is created

## Fall-off Curve Types

### Linear
```
probability = 1 - (distance / maxDistance)^(1/strength)
```
Simple linear decrease. Lower strength = more gradual, higher = steeper.

### Exponential
```
probability = exp(-distance * strength / maxDistance)
```
Exponential decay. Higher strength = faster decay.

### Gaussian
```
probability = exp(-(distance^2) / (2 * sigma^2))
```
Smooth bell curve distribution. Higher strength = narrower distribution.

### Smoothstep (Recommended)
```
t = (distance / maxDistance)^(1/strength)
probability = 1 - (3t^2 - 2t^3)
```
Smooth cubic interpolation. Provides the most natural-looking results.

## Building the Plugin

### Enable in CMake

When configuring CloudCompare with CMake, enable the plugin:

```bash
cmake -DPLUGIN_STANDARD_QFEATHEREDEDGE=ON ...
```

Or use `ccmake` or `cmake-gui` to enable `PLUGIN_STANDARD_QFEATHEREDEDGE`.

### Build

```bash
cmake --build . --target QFEATHEREDEDGE_PLUGIN
```

Or simply build the entire CloudCompare project and the plugin will be included.

## Usage

1. **Load a Point Cloud**: Open a point cloud in CloudCompare
2. **Select the Cloud**: Ensure the cloud is selected in the DB tree
3. **Launch Plugin**: Go to `Plugins > Feathered Edge`
4. **Set Origin Point**:
   - Click "Use Cloud Center" for automatic center selection
   - Or manually enter coordinates
5. **Adjust Parameters**:
   - **Max Distance**: How far the effect extends (default: 25% of cloud diagonal)
   - **Fall-off Type**: Choose the curve type (Smoothstep recommended)
   - **Strength**: Controls the steepness (default: 1.0)
   - **Random Seed**: For reproducibility (0 = random)
6. **Click OK**: Plugin processes the cloud and creates a new feathered version

## Tips for Best Results

- **Start with Cloud Center**: Use the cloud center as origin for symmetric feathering
- **Try Smoothstep First**: Usually gives the most natural-looking results
- **Adjust Max Distance**:
  - Larger values = more gradual feathering
  - Smaller values = sharper transition
- **Tune Strength**:
  - Values < 1.0 = gentler fade
  - Values > 1.0 = more aggressive fade
  - Start at 1.0 and adjust to taste
- **Use Random Seed**: Set a non-zero seed if you need reproducible results

## Example Scenarios

### Terrestrial Scan with Hard Edge
- **Problem**: Scanner couldn't reach certain areas, creating a hard cutoff
- **Solution**: Set origin at the visible part of the cloud, max distance to cover the edge region
- **Recommended**: Smoothstep with strength 1.0-1.5

### Photogrammetry Missing Data
- **Problem**: Areas with no photo coverage have abrupt boundaries
- **Solution**: Position origin in well-covered area, use larger max distance
- **Recommended**: Gaussian with strength 2.0-3.0 for soft fade

### Creating Vignette Effect
- **Problem**: Want to draw attention to center of cloud
- **Solution**: Origin at cloud center, max distance = half the cloud size
- **Recommended**: Exponential with strength 2.0-4.0

## Limitations & Future Enhancements

### Current Limitations
- Interactive point picking not yet implemented (use manual coordinates or cloud center)
- Single origin point only
- No preview mode

### Planned Enhancements
- Interactive point picking using ccPickingHub
- Multiple origin points for complex feathering patterns
- Preview mode showing which points will be removed
- Distance scalar field visualization
- Undo/redo support

## Technical Details

### Algorithm Complexity
- **Time**: O(n) where n is the number of points
- **Space**: O(n) for the output ReferenceCloud

### Memory Usage
- Minimal additional memory required
- Uses ReferenceCloud (stores indices only)
- New cloud created via partialClone

### Thread Safety
- Plugin is single-threaded
- Uses std::mt19937 for random number generation
- Safe to use with CloudCompare's progress dialog

## Troubleshooting

### "No points were removed"
- Increase the maximum distance parameter
- Try a different fall-off type
- Adjust the strength parameter

### "Not enough memory"
- Close other applications
- Process smaller sections of the cloud
- Reduce the number of points using CloudCompare's subsample tools first

### Plugin doesn't appear in menu
- Ensure plugin was enabled during CMake configuration
- Rebuild CloudCompare
- Check that plugin DLL/SO is in the plugins directory

## License

This plugin is released under the GNU General Public License v2 or later, consistent with CloudCompare's licensing.

## Contributing

Contributions are welcome! Areas where help would be appreciated:
- Interactive point picking implementation
- Additional fall-off curve types
- Preview mode
- Performance optimizations
- Custom icon design

## Credits

Developed for the CloudCompare community to address common aesthetic issues with scanned point clouds.
