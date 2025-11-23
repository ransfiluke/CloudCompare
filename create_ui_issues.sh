#!/bin/bash
# Script to create GitHub labels and issues for CloudCompare UI Modernization
# Run this script from the CloudCompare repository root

set -e  # Exit on error

echo "Creating GitHub labels for UI Modernization..."

# Create Priority Labels
gh label create "P0-critical" --description "Critical priority - breaks functionality" --color "d73a4a" --force 2>/dev/null || true
gh label create "P1-high" --description "High priority - significant UX impact" --color "ff6b6b" --force 2>/dev/null || true
gh label create "P2-medium" --description "Medium priority - polish and consistency" --color "ffa502" --force 2>/dev/null || true
gh label create "P3-low" --description "Low priority - nice-to-have improvements" --color "fffa65" --force 2>/dev/null || true

# Create Category Labels
gh label create "ui-theming" --description "UI theming and dark mode support" --color "c5def5" --force 2>/dev/null || true
gh label create "ui-accessibility" --description "Accessibility improvements (keyboard, screen readers)" --color "1d76db" --force 2>/dev/null || true
gh label create "ui-responsive" --description "Responsive design and layout" --color "0e8a16" --force 2>/dev/null || true
gh label create "ui-consistency" --description "UI consistency and standardization" --color "bfd4f2" --force 2>/dev/null || true
gh label create "ui-documentation" --description "UI guidelines and documentation" --color "f9d0c4" --force 2>/dev/null || true
gh label create "ui-automation" --description "Automated UI testing and linting" --color "5319e7" --force 2>/dev/null || true

# Create Effort Labels
gh label create "effort-small" --description "Small effort (< 2 hours)" --color "d4c5f9" --force 2>/dev/null || true
gh label create "effort-medium" --description "Medium effort (2-8 hours)" --color "c2e0c6" --force 2>/dev/null || true

# Create General Labels
gh label create "ui/ux" --description "User interface and user experience" --color "0075ca" --force 2>/dev/null || true
gh label create "good-first-issue" --description "Good for newcomers" --color "7057ff" --force 2>/dev/null || true

echo "✓ Labels created successfully"
echo ""
echo "Creating GitHub issues for UI Modernization tasks..."
echo ""

# ============================================================================
# PHASE 1: Critical Fixes (P0) - Hardcoded Colors
# ============================================================================

gh issue create --title "Remove hardcoded colors from core dialogs (Batch 1/4)" --label "P0-critical,ui-theming,ui/ux,effort-small,good-first-issue" --body "## Description
Remove hardcoded colors from 5 core dialog UI files that break theming and dark mode support.

## Problem
These dialogs contain hardcoded colors in styleSheet properties that don't adapt to theme changes, causing poor visibility in dark mode and breaking the theming system.

## Affected Files
1. \`qCC/ui_templates/boundingBoxEditorDlg.ui\` - Line 226 (red warning text)
2. \`qCC/ui_templates/pointPairRegistrationDlg.ui\` - Line 22 (red background checkbox)
3. \`qCC/ui_templates/histogramDlg.ui\` - Line 17 (white background)
4. \`libs/qCC_io/ui/openAsciiFileDlg.ui\` - Line 110 (grey text)
5. \`qCC/ui_templates/volumeCalcDlg.ui\` - (various hardcoded colors)

## Changes Required
- Remove all \`styleSheet\` properties containing hardcoded color values
- For warning/error text: Use dynamic palette-based styling in C++ code instead
- For semantic highlights: Implement theme-aware CSS classes

## Acceptance Criteria
- [ ] All 5 files have hardcoded color stylesheets removed
- [ ] Dialogs display correctly in Light theme
- [ ] Dialogs display correctly in Dark theme
- [ ] Warning/error messages are still visually distinct (if applicable)
- [ ] No visual regression in normal use cases

## Testing Steps
1. Open CloudCompare with Light theme
2. Open each affected dialog and verify proper appearance
3. Switch to Dark theme (Display Settings)
4. Re-open each dialog and verify proper appearance
5. Test any warning/error states (e.g., trigger validation errors)

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 1.1
- Related to issue #<theming-issues>

## Estimated Effort
~30 minutes per file (2.5 hours total)"

gh issue create --title "Remove hardcoded colors from IO dialogs (Batch 2/4)" --label "P0-critical,ui-theming,ui/ux,effort-small,good-first-issue" --body "## Description
Remove hardcoded colors from 5 IO-related dialog UI files that break theming and dark mode support.

## Problem
These dialogs contain hardcoded colors in styleSheet properties that don't adapt to theme changes.

## Affected Files
1. \`libs/qCC_io/ui/globalShiftAndScaleDlg.ui\`
2. \`qCC/ui_templates/globalShiftSettingsDlg.ui\`
3. \`qCC/ui_templates/registrationDlg.ui\`
4. \`qCC/ui_templates/rasterizeDlg.ui\`
5. \`qCC/ui_templates/comparisonDlg.ui\`

## Changes Required
- Remove all \`styleSheet\` properties containing hardcoded color values
- Document pattern for semantic colors in code comments

## Acceptance Criteria
- [ ] All 5 files have hardcoded color stylesheets removed
- [ ] Dialogs display correctly in both Light and Dark themes
- [ ] No visual regression in normal use cases
- [ ] Color usage patterns documented where applicable

## Testing Steps
1. Test each dialog in both Light and Dark themes
2. Verify IO operations complete successfully
3. Check that visual hierarchy is maintained

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 1.2

## Estimated Effort
~30 minutes per file (2.5 hours total)"

gh issue create --title "Remove hardcoded colors from plugin dialogs (Batch 3/4)" --label "P0-critical,ui-theming,ui/ux,effort-small" --body "## Description
Remove hardcoded colors from 5 plugin dialog UI files that break theming and dark mode support.

## Problem
Plugin dialogs contain hardcoded colors that don't respect the application theme.

## Affected Files
1. \`libs/CCPluginAPI/ui/colorScaleEditorDlg.ui\`
2. \`qCC/ui_templates/exportCoordToSFDlg.ui\`
3. \`qCC/ui_templates/clippingBoxRepeatDlg.ui\`
4. \`qCC/ui_templates/ccDrawNormalsWidget.ui\`
5. Other plugin dialogs with hardcoded colors (to be identified)

## Changes Required
- Remove hardcoded color stylesheets
- Ensure color scale editor uses dynamic colors appropriately
- Verify plugin functionality is not affected

## Acceptance Criteria
- [ ] All identified files have hardcoded color stylesheets removed
- [ ] Plugin dialogs display correctly in both themes
- [ ] Color scale editor maintains proper functionality
- [ ] No visual regression in plugin features

## Testing Steps
1. Load plugins and test each affected dialog
2. Verify in both Light and Dark themes
3. Test plugin-specific functionality (color scales, exports, etc.)

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 1.3

## Estimated Effort
~30 minutes per file (2.5 hours total)"

gh issue create --title "Remove hardcoded colors from remaining dialogs (Batch 4/4)" --label "P0-critical,ui-theming,ui/ux,effort-small" --body "## Description
Complete removal of all remaining hardcoded colors from UI files across the codebase.

## Problem
Final cleanup to ensure 100% theme compatibility across all dialogs.

## Changes Required
- Identify any remaining UI files with hardcoded colors
- Remove all hardcoded color stylesheets
- Perform final verification across all themes

## Acceptance Criteria
- [ ] No UI files contain hardcoded colors in stylesheets
- [ ] Full regression test passes in both themes
- [ ] Grep search confirms no remaining hardcoded colors: \`grep -r \"styleSheet.*color:\" qCC/ libs/ plugins/\`

## Testing Steps
1. Run automated search for remaining hardcoded colors
2. Test all dialogs in both Light and Dark themes
3. Document any legitimate use cases for hardcoded colors (if any)

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 1.4

## Estimated Effort
~2 hours (search, fix, verify)"

# ============================================================================
# PHASE 2: Dialog Button Standardization (P0-P1)
# ============================================================================

gh issue create --title "Convert manual buttons to QDialogButtonBox in core dialogs (Batch 1/3)" --label "P0-critical,ui-consistency,ui/ux,effort-small" --body "## Description
Convert 3 core dialogs from manual QPushButton implementations to standard QDialogButtonBox.

## Problem
Manual button implementations lose platform-native button ordering (OK/Cancel positions swap on macOS vs Windows) and don't benefit from Qt's automatic keyboard shortcuts and accessibility features.

## Affected Files
1. \`qCC/ui_templates/boundingBoxEditorDlg.ui\` (Lines 599-625)
2. \`qCC/ui_templates/comparisonDlg.ui\`
3. \`libs/CCAppCommon/ui/displaySettingsDlg.ui\`

## Changes Required

### UI Files (.ui)
- Replace manual QPushButton widgets with QDialogButtonBox
- Use standard roles: \`QDialogButtonBox::Ok | QDialogButtonBox::Cancel\`
- For special buttons (e.g., \"Default\", \"Last\" in boundingBoxEditorDlg), add as custom buttons using \`addButton()\`

### C++ Files (.cpp/.h)
- Update signal/slot connections:
  - Old: \`connect(okButton, &QPushButton::clicked, this, &Dialog::accept)\`
  - New: \`connect(buttonBox, &QDialogButtonBox::accepted, this, &Dialog::accept)\`
  - New: \`connect(buttonBox, &QDialogButtonBox::rejected, this, &Dialog::reject)\`
- Handle custom button clicks via \`QDialogButtonBox::clicked(QAbstractButton*)\` signal

## Example Code

### In .ui file (XML):
\`\`\`xml
<widget class=\"QDialogButtonBox\" name=\"buttonBox\">
  <property name=\"standardButtons\">
    <set>QDialogButtonBox::Cancel|QDialogButtonBox::Ok</set>
  </property>
</widget>
\`\`\`

### In .cpp file:
\`\`\`cpp
// For standard buttons
connect(buttonBox, &QDialogButtonBox::accepted, this, &Dialog::accept);
connect(buttonBox, &QDialogButtonBox::rejected, this, &Dialog::reject);

// For custom buttons (e.g., \"Default\" button)
QPushButton* defaultButton = buttonBox->addButton(tr(\"Default\"), QDialogButtonBox::ResetRole);
connect(defaultButton, &QPushButton::clicked, this, &Dialog::onDefaultClicked);
\`\`\`

## Acceptance Criteria
- [ ] All 3 dialogs use QDialogButtonBox instead of manual buttons
- [ ] Button ordering follows platform conventions (test on Windows, macOS, Linux)
- [ ] Keyboard shortcuts work (Enter = OK, Esc = Cancel)
- [ ] Custom button functionality preserved (if applicable)
- [ ] No behavioral regressions

## Testing Steps
1. Open each dialog and verify button layout
2. Click OK button - dialog should accept
3. Click Cancel button - dialog should reject
4. Press Enter - should trigger OK
5. Press Escape - should trigger Cancel
6. Test custom buttons (e.g., \"Default\" in boundingBoxEditorDlg)
7. Test on multiple platforms if possible (Windows/macOS/Linux)

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 2.1
- Qt Documentation: https://doc.qt.io/qt-6/qdialogbuttonbox.html

## Estimated Effort
~30-45 minutes per dialog (2 hours total)"

gh issue create --title "Convert manual buttons to QDialogButtonBox in IO dialogs (Batch 2/3)" --label "P1-high,ui-consistency,ui/ux,effort-small,good-first-issue" --body "## Description
Convert 3 IO-related dialogs from manual QPushButton implementations to standard QDialogButtonBox.

## Problem
Manual button implementations don't follow platform conventions and lack automatic accessibility features.

## Affected Files
1. \`libs/qCC_io/ui/importDBFFieldDlg.ui\`
2. \`libs/qCC_io/ui/openAsciiFileDlg.ui\`
3. \`libs/qCC_io/ui/openPlyFileDlg.ui\`

## Changes Required
- Replace manual QPushButton elements with QDialogButtonBox
- Update corresponding .cpp signal handlers
- Use standard button roles

## Acceptance Criteria
- [ ] All 3 dialogs use QDialogButtonBox
- [ ] File import operations work correctly
- [ ] Button ordering follows platform conventions
- [ ] Keyboard shortcuts work (Enter/Escape)

## Testing Steps
1. Test file import workflow for each dialog
2. Verify button behavior and keyboard shortcuts
3. Test on multiple platforms if possible

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 2.2
- See previous batch issue for code examples

## Estimated Effort
~30-45 minutes per dialog (2 hours total)"

gh issue create --title "Convert manual buttons to QDialogButtonBox in plugin dialogs (Batch 3/3)" --label "P1-high,ui-consistency,ui/ux,effort-small" --body "## Description
Convert 4 plugin dialogs from manual QPushButton implementations to standard QDialogButtonBox.

## Problem
Plugin dialogs don't follow platform button ordering conventions.

## Affected Files
1. \`plugins/core/IO/qLASIO/ui/lasopendialog.ui\`
2. \`plugins/core/IO/qPDALIO/ui/openLASFileDlg.ui\`
3. \`plugins/core/Standard/qBroom/ui/broomDlg.ui\`
4. \`qCC/ui_templates/filterByValueDlg.ui\` (special case: Export/Split buttons)

## Special Case: filterByValueDlg
This dialog has non-standard buttons (\"Export\", \"Split\", \"Cancel\"). Handle as follows:
\`\`\`cpp
QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
QPushButton* exportButton = buttonBox->addButton(tr(\"Export\"), QDialogButtonBox::ActionRole);
QPushButton* splitButton = buttonBox->addButton(tr(\"Split\"), QDialogButtonBox::ActionRole);
buttonBox->addButton(QDialogButtonBox::Cancel);

connect(exportButton, &QPushButton::clicked, this, &Dialog::onExport);
connect(splitButton, &QPushButton::clicked, this, &Dialog::onSplit);
connect(buttonBox, &QDialogButtonBox::rejected, this, &Dialog::reject);
\`\`\`

## Acceptance Criteria
- [ ] All 4 dialogs use QDialogButtonBox
- [ ] Plugin functionality preserved
- [ ] Special button actions work correctly (Export/Split in filterByValueDlg)
- [ ] Platform-native button ordering

## Testing Steps
1. Load and test each plugin dialog
2. Verify special actions (export, split, etc.)
3. Test keyboard shortcuts

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 2.3

## Estimated Effort
~45 minutes per dialog (3 hours total)"

# ============================================================================
# PHASE 3: Responsive Design (P1)
# ============================================================================

gh issue create --title "Remove hardcoded geometry from simple dialogs (Batch 1/4)" --label "P1-high,ui-responsive,ui/ux,effort-small,good-first-issue" --body "## Description
Remove hardcoded window geometry from 5 simple input dialogs to enable responsive design and user preferences.

## Problem
Hardcoded geometry prevents:
- Proper responsive design on different screen sizes
- User resizing and preference saving
- Adaptation to different DPI settings
- Proper content-based sizing

## Affected Files
1. \`qCC/ui_templates/askThreeDoubleValuesDlg.ui\` (452x89)
2. \`qCC/ui_templates/filterByValueDlg.ui\` (432x99)
3. \`qCC/ui_templates/askOneIntValueDlg.ui\` (if hardcoded)
4. \`qCC/ui_templates/askTwoDoubleValuesDlg.ui\` (if hardcoded)
5. Other simple input dialogs (identify during implementation)

## Changes Required

Remove this from .ui files:
\`\`\`xml
<property name=\"geometry\">
  <rect>
    <x>0</x>
    <y>0</y>
    <width>452</width>
    <height>89</height>
  </rect>
</property>
\`\`\`

Ensure proper sizePolicy is set:
\`\`\`xml
<property name=\"sizePolicy\">
  <sizepolicy hsizetype=\"Preferred\" vsizetype=\"Preferred\">
    <horstretch>0</horstretch>
    <verstretch>0</verstretch>
  </sizepolicy>
</property>
\`\`\`

## Acceptance Criteria
- [ ] Hardcoded \`<rect>\` geometry removed from all 5 files
- [ ] Dialogs size correctly based on content
- [ ] Dialogs display all content properly at default size
- [ ] User can resize dialogs (if appropriate)
- [ ] Dialog size preferences are remembered between sessions
- [ ] Works correctly on HiDPI displays

## Testing Steps
1. Open each dialog
2. Verify all content is visible at initial size
3. Resize dialog (if applicable)
4. Close and reopen - verify size is remembered
5. Test on different DPI settings (100%, 125%, 150%, 200%)
6. Test on different screen resolutions

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 3.1

## Estimated Effort
~20 minutes per file (1.5 hours total)"

gh issue create --title "Remove hardcoded geometry from medium dialogs (Batch 2/4)" --label "P1-high,ui-responsive,ui/ux,effort-small" --body "## Description
Remove hardcoded window geometry from 5 medium-sized dialogs.

## Affected Files
1. \`qCC/ui_templates/alignDlg.ui\` (600x361)
2. \`qCC/ui_templates/comparisonDlg.ui\` (450x400)
3. \`qCC/ui_templates/registrationDlg.ui\` (411x458)
4. \`libs/CCAppCommon/ui/displaySettingsDlg.ui\` (514x580)
5. Other medium-sized dialogs

## Changes Required
- Remove hardcoded geometry
- Set reasonable minimumSize only if absolutely needed
- Prefer dynamic sizing based on content

## Acceptance Criteria
- [ ] Hardcoded geometry removed from all 5 files
- [ ] Dialogs display all content properly
- [ ] No content is cut off or requires scrolling inappropriately
- [ ] Reasonable initial sizes on typical displays

## Testing Steps
1. Test each dialog at various screen sizes
2. Verify all tabs/sections are accessible (for tabbed dialogs)
3. Test on both standard and HiDPI displays

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 3.2

## Estimated Effort
~30 minutes per file (2.5 hours total)"

gh issue create --title "Remove hardcoded geometry from complex dialogs (Batch 3/4)" --label "P1-high,ui-responsive,ui/ux,effort-medium" --body "## Description
Remove hardcoded window geometry from 5 complex dialogs with extensive content.

## Affected Files
1. \`qCC/ui_templates/rasterizeDlg.ui\` (1024x800)
2. \`qCC/ui_templates/volumeCalcDlg.ui\` (1000x887)
3. \`qCC/ui_templates/boundingBoxEditorDlg.ui\` (600x650)
4. \`qCC/ui_templates/histogramDlg.ui\` (800x600)
5. \`libs/qCC_io/ui/openAsciiFileDlg.ui\` (1008x657)

## Changes Required
- Remove hardcoded geometry
- May need layout adjustments to ensure proper content flow
- Consider adding scroll areas if content is extensive
- May need to set minimumSize for usability

## Acceptance Criteria
- [ ] Hardcoded geometry removed
- [ ] All content is accessible and properly laid out
- [ ] Dialogs are usable on smaller displays (1366x768 minimum)
- [ ] Complex widgets (histograms, preview areas) size appropriately
- [ ] Scroll areas added where appropriate

## Testing Steps
1. Test on various screen sizes (laptop, desktop, HiDPI)
2. Verify complex widgets render correctly
3. Test all tabs and sections for accessibility
4. Verify scrolling works correctly if added

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 3.3

## Estimated Effort
~45 minutes per file (4 hours total)"

gh issue create --title "Remove hardcoded geometry from plugin dialogs (Batch 4/4)" --label "P1-high,ui-responsive,ui/ux,effort-small" --body "## Description
Remove hardcoded window geometry from plugin dialogs.

## Affected Files
1. \`plugins/core/Standard/qM3C2/ui/qM3C2Dialog.ui\` (755x490)
2. Other plugin dialogs with hardcoded sizes (identify during audit)

## Changes Required
- Remove hardcoded geometry from plugin dialogs
- Ensure plugins follow same responsive design standards as core

## Acceptance Criteria
- [ ] All plugin dialogs have hardcoded geometry removed
- [ ] Plugin dialogs work correctly at various sizes
- [ ] Plugin functionality is not affected

## Testing Steps
1. Load each plugin
2. Test affected dialogs at various screen sizes
3. Verify plugin operations complete successfully

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 3.4

## Estimated Effort
~30 minutes per file (2 hours total)"

gh issue create --title "Audit and remove unnecessary hardcoded min/max sizes" --label "P1-high,ui-responsive,ui/ux,effort-small" --body "## Description
Review and remove unnecessary minimumSize and maximumSize constraints from widgets across the codebase.

## Problem
Many dialogs specify hardcoded minimumSize or maximumSize that limit flexibility without clear benefit.

## Changes Required
- Audit all UI files for minimumSize/maximumSize properties
- Remove if not essential
- Replace with sizePolicy where appropriate
- Keep only if there's a valid reason (document in code comment)

## Examples of Valid Use Cases
- Minimum size for image display areas
- Maximum width for readability of text fields
- Preventing dialogs from becoming too small to be usable

## Acceptance Criteria
- [ ] All unnecessary min/max sizes removed
- [ ] Valid min/max sizes documented with comments
- [ ] Dialogs remain usable at various sizes
- [ ] No visual regressions

## Testing Steps
1. Test affected dialogs at various sizes
2. Verify usability is maintained or improved
3. Check edge cases (very small/large windows)

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 3.5
- Affected files: 20+ (can be done in sub-batches)

## Estimated Effort
~15 minutes per file (5-6 hours total, can be split)"

# ============================================================================
# PHASE 4: Accessibility Improvements (P1)
# ============================================================================

gh issue create --title "Add tab order to core dialogs (Batch 1/6)" --label "P1-high,ui-accessibility,ui/ux,effort-small,good-first-issue" --body "## Description
Add logical tab stop definitions to 10 frequently-used core dialogs for better keyboard navigation.

## Problem
Only 1 out of 100+ dialogs defines tab stops, making keyboard navigation unpredictable and frustrating for keyboard-only users.

## Example (from subsamplingDlg.ui)
\`\`\`xml
<tabstops>
  <tabstop>firstWidget</tabstop>
  <tabstop>secondWidget</tabstop>
  <tabstop>thirdWidget</tabstop>
  <tabstop>okButton</tabstop>
  <tabstop>cancelButton</tabstop>
</tabstops>
\`\`\`

## Guidelines
- Order: top to bottom, left to right
- Include all interactive widgets
- Dialog buttons (OK/Cancel) should be last
- Skip decorative/read-only widgets

## Affected Files
Top 10 most-used dialogs (to be determined):
- [ ] Identify through usage analysis
- [ ] Prioritize main window dialogs
- [ ] Include common operations (open, save, import, export)

## Acceptance Criteria
- [ ] All 10 dialogs have \`<tabstops>\` section
- [ ] Tab order is logical and intuitive
- [ ] All interactive widgets included
- [ ] Tab navigation tested with keyboard only

## Testing Steps
1. Open dialog
2. Press Tab repeatedly
3. Verify focus moves in logical order
4. Verify all interactive widgets are reached
5. Verify Tab wraps back to first widget
6. Test Shift+Tab (reverse direction)

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 4.1

## Estimated Effort
~10 minutes per dialog (1.5 hours total)"

gh issue create --title "Add tab order to IO dialogs (Batch 2/6)" --label "P1-high,ui-accessibility,ui/ux,effort-small,good-first-issue" --body "## Description
Add logical tab stop definitions to 10 IO-related dialogs.

## Affected Files
All dialogs in \`libs/qCC_io/ui/\`:
- [ ] List files during implementation
- [ ] Include file open/save dialogs
- [ ] Include import/export dialogs

## Changes Required
Add \`<tabstops>\` section with logical tab order

## Acceptance Criteria
- [ ] All IO dialogs have logical tab order
- [ ] File operations are fully keyboard-accessible
- [ ] Tab order tested with keyboard only

## Testing Steps
1. Test each dialog with keyboard-only navigation
2. Verify file selection works via keyboard
3. Test browse buttons, path fields, options

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 4.2
- See Batch 1 for examples

## Estimated Effort
~10 minutes per dialog (1.5 hours total)"

gh issue create --title "Add tab order to remaining dialogs (Batches 3-6)" --label "P1-high,ui-accessibility,ui/ux,effort-medium" --body "## Description
Add logical tab stop definitions to all remaining dialogs (60+ files).

## Strategy
- Process in batches of 10-15 files
- Can be split into multiple PRs
- Use consistent patterns from previous batches

## Affected Files
All remaining dialogs in:
- \`qCC/ui_templates/\`
- \`plugins/*/ui/\`
- \`libs/*/ui/\`

## Acceptance Criteria
- [ ] 100% of dialogs have defined tab stops
- [ ] Consistent tab order patterns across application
- [ ] Automated test confirms all dialogs have tab stops

## Testing Strategy
- Spot-check random dialogs for correct tab order
- Focus testing on complex dialogs
- Automate verification where possible

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 4.3

## Estimated Effort
~10 minutes per dialog (10-12 hours total)

## Implementation Notes
This can be split into 4 separate issues for easier tracking:
- Batch 3: Dialogs A-M
- Batch 4: Dialogs N-R
- Batch 5: Dialogs S-Z
- Batch 6: Plugin dialogs"

gh issue create --title "Add label buddies to core dialogs (Batch 1/5)" --label "P1-high,ui-accessibility,ui/ux,effort-small,good-first-issue" --body "## Description
Associate QLabel widgets with their corresponding input widgets in 10 core dialogs.

## Problem
Only 1 dialog properly uses label buddies. This breaks:
- Click-to-focus (clicking label doesn't focus input)
- Screen reader navigation
- Accessibility standards compliance

## Example
\`\`\`xml
<widget class=\"QLabel\" name=\"nameLabel\">
  <property name=\"text\">
    <string>&amp;Name:</string>
  </property>
  <property name=\"buddy\">
    <cstring>nameLineEdit</cstring>
  </property>
</widget>
<widget class=\"QLineEdit\" name=\"nameLineEdit\"/>
\`\`\`

## Benefits
- Clicking label focuses the input
- Screen readers announce labels correctly
- Keyboard shortcuts work (if mnemonic present)

## Affected Files
Top 10 most-used dialogs (same as tab order Batch 1)

## Acceptance Criteria
- [ ] All QLabel widgets have valid buddy associations
- [ ] Buddy points to correct input widget (not groupbox or container)
- [ ] Clicking label focuses corresponding input
- [ ] Tested with screen reader

## Testing Steps
1. Click each label in dialog
2. Verify focus moves to correct input widget
3. Test with screen reader (NVDA, VoiceOver, Orca)
4. Verify label is announced when input is focused

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 4.4
- Qt Docs: https://doc.qt.io/qt-6/qlabel.html#setBuddy

## Estimated Effort
~15 minutes per dialog (2.5 hours total)"

gh issue create --title "Add label buddies to remaining dialogs (Batches 2-5)" --label "P1-high,ui-accessibility,ui/ux,effort-medium" --body "## Description
Associate QLabel widgets with their corresponding input widgets in all remaining dialogs (50+ files).

## Strategy
- Process in batches of 10-12 files
- Can be split into 4 separate PRs

## Changes Required
Add \`<buddy>\` property to all QLabel widgets that describe input fields

## Acceptance Criteria
- [ ] 100% of label-input pairs have buddy associations
- [ ] No labels point to groupboxes or containers (common error)
- [ ] Automated test verifies all labels have valid buddies

## Testing Strategy
- Spot-check random dialogs
- Create automated test to verify buddies exist and are valid

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 4.5

## Estimated Effort
~15 minutes per dialog (12-15 hours total)

## Implementation Notes
Suggest splitting into 4 issues:
- Batch 2: Core dialogs (10-12 files)
- Batch 3: IO dialogs (10-12 files)
- Batch 4: Plugin dialogs (10-12 files)
- Batch 5: Remaining dialogs (10-12 files)"

gh issue create --title "Add keyboard shortcuts (mnemonics) to core dialogs (Batch 1/5)" --label "P1-high,ui-accessibility,ui/ux,effort-small,good-first-issue" --body "## Description
Add keyboard mnemonics (Alt+letter shortcuts) to labels and buttons in 10 core dialogs.

## Problem
Only 2 dialogs use keyboard shortcuts, making keyboard-only navigation difficult.

## How It Works
Add '&' before the shortcut letter in text:
\`\`\`xml
<property name=\"text\">
  <string>&amp;Name:</string>  <!-- Alt+N focuses this field -->
</property>
\`\`\`

## Common Patterns
- Buttons: \`&OK\`, \`&Cancel\`, \`&Apply\`, \`&Browse\`
- Actions: \`&Export\`, \`&Import\`, \`&Save\`, \`&Load\`
- Fields: \`&Name\`, \`&Value\`, \`&Type\`, \`&Options\`

## Important Rules
1. No duplicates within a dialog
2. Use first letter when possible
3. Use distinctive letter if conflict
4. Common buttons should be consistent across dialogs

## Affected Files
Top 10 most-used dialogs (same as previous batches)

## Acceptance Criteria
- [ ] All labels and buttons have mnemonics
- [ ] No duplicate mnemonics within each dialog
- [ ] Alt+letter activates correct control
- [ ] Mnemonics work with label buddies (double benefit)

## Testing Steps
1. Press Alt to show underlined letters
2. Press Alt+letter for each mnemonic
3. Verify correct control activates
4. Test all controls in dialog

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 4.6

## Estimated Effort
~15 minutes per dialog (2.5 hours total)"

gh issue create --title "Add keyboard shortcuts to remaining dialogs (Batches 2-5)" --label "P1-high,ui-accessibility,ui/ux,effort-medium" --body "## Description
Add keyboard mnemonics to all remaining dialogs (50+ files).

## Strategy
- Process in batches of 10-12 files
- Ensure consistency with mnemonics from previous batches
- Avoid conflicts within each dialog

## Changes Required
Add '&' character before shortcut letter in all button/label text

## Acceptance Criteria
- [ ] 100% of dialogs have mnemonics
- [ ] No conflicts within dialogs
- [ ] Consistent mnemonics for common actions across dialogs
- [ ] Automated test verifies no duplicate mnemonics per dialog

## Testing Strategy
- Create script to check for duplicate mnemonics
- Spot-test random dialogs
- Focus on consistency of common buttons

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 4.7

## Estimated Effort
~15 minutes per dialog (12-15 hours total)

## Implementation Notes
Suggest splitting into 4 issues for easier tracking"

# ============================================================================
# PHASE 5: Tooltip Enhancement (P1-P2)
# ============================================================================

gh issue create --title "Create tooltip guidelines document" --label "P1-high,ui-documentation,ui/ux,effort-small,good-first-issue" --body "## Description
Create comprehensive guidelines for writing and adding tooltips to UI elements.

## Deliverable
\`docs/UI_TOOLTIP_GUIDELINES.md\`

## Content Required
1. **When to add tooltips**
   - All non-obvious controls
   - Technical parameters
   - Buttons with icons only
   - Advanced/expert features

2. **When NOT to add tooltips**
   - Redundant with visible label
   - Standard buttons (OK/Cancel)
   - Self-explanatory controls

3. **Writing style**
   - Concise (1-2 sentences max)
   - Helpful, not redundant
   - Describe purpose, not just label
   - Include keyboard shortcuts if applicable

4. **Examples**
   - Good tooltip: \"Maximum distance for point matching (in project units). Higher values find more matches but may reduce accuracy.\"
   - Bad tooltip: \"Maximum distance\" (redundant with label)

5. **Template phrases**
   - For spinboxes: \"[Purpose]. Valid range: X to Y. Default: Z.\"
   - For checkboxes: \"[Effect when enabled]. [Optional: consequence if disabled]\"
   - For buttons: \"[Action performed]. [Optional: keyboard shortcut]\"

## Acceptance Criteria
- [ ] Document created with all required sections
- [ ] 10+ good/bad examples included
- [ ] Guidelines are clear and actionable
- [ ] Reviewed by team

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 5.1

## Estimated Effort
2-3 hours"

gh issue create --title "Add tooltips to main window toolbars and menus" --label "P1-high,ui-accessibility,ui/ux,effort-small" --body "## Description
Add helpful tooltips to all toolbar buttons and menu items in the main window.

## Affected Files
- \`qCC/ui_templates/mainWindow.ui\`
- \`qCC/mainwindow.cpp\` (for dynamic tooltips)

## Changes Required
- Add tooltips to all toolbar buttons
- Add status tips to menu items
- Include keyboard shortcuts in tooltips where applicable

## Example
\`\`\`xml
<widget class=\"QToolButton\" name=\"openButton\">
  <property name=\"toolTip\">
    <string>Open point cloud file (Ctrl+O)</string>
  </property>
  <property name=\"statusTip\">
    <string>Open a point cloud from file</string>
  </property>
</widget>
\`\`\`

## Acceptance Criteria
- [ ] All toolbar buttons have tooltips
- [ ] Tooltips include keyboard shortcuts
- [ ] Menu items have helpful status tips
- [ ] Tooltips follow guidelines document

## Testing Steps
1. Hover over each toolbar button
2. Verify helpful tooltip appears
3. Check menu items show status tips in status bar
4. Verify keyboard shortcuts are accurate

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 5.2
- Depends on: Tooltip guidelines document

## Estimated Effort
1-2 hours"

gh issue create --title "Add tooltips to core dialogs (Batches 1-2)" --label "P2-medium,ui-accessibility,ui/ux,effort-medium" --body "## Description
Add helpful tooltips to all interactive widgets in 20 core dialogs.

## Strategy
- Focus on frequently-used dialogs first
- Process 10 dialogs per batch (2 batches)
- Prioritize non-obvious controls

## Changes Required
Add tooltips to:
- Spinboxes (include valid ranges)
- Checkboxes (explain effect)
- Combo boxes (describe options)
- Buttons (describe action)
- Radio buttons (explain choice)

## Acceptance Criteria
- [ ] All interactive widgets have tooltips
- [ ] Tooltips follow guidelines document
- [ ] Tooltips are helpful and non-redundant
- [ ] Technical parameters explained clearly

## Testing Steps
1. Hover over each control
2. Verify tooltip is helpful
3. Check for redundancy with visible labels
4. Verify accuracy of information

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 5.3
- Depends on: Tooltip guidelines document

## Estimated Effort
~20 minutes per dialog (6-7 hours total for both batches)"

gh issue create --title "Add tooltips to remaining dialogs (Batches 3-8)" --label "P2-medium,ui-accessibility,ui/ux,effort-medium" --body "## Description
Add helpful tooltips to all interactive widgets in 60+ remaining dialogs.

## Strategy
- Process in batches of 10 dialogs
- Can be split into 6 separate issues/PRs
- Maintain consistency with previous batches

## Affected Files
All remaining dialogs in:
- \`qCC/ui_templates/\`
- \`plugins/*/ui/\`
- \`libs/*/ui/\`

## Acceptance Criteria
- [ ] 100% tooltip coverage on interactive widgets
- [ ] Consistent terminology across dialogs
- [ ] Quality spot-checks pass

## Testing Strategy
- Automated check for widgets without tooltips
- Manual spot-check for quality
- Focus on technical/complex dialogs

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 5.4

## Estimated Effort
~20 minutes per dialog (20-25 hours total)

## Implementation Notes
Suggest splitting into 6 issues:
- Batch 3-8: 10 dialogs each"

# ============================================================================
# PHASE 6: Layout Consistency (P2)
# ============================================================================

gh issue create --title "Create layout standards document" --label "P2-medium,ui-documentation,ui/ux,effort-small,good-first-issue" --body "## Description
Create comprehensive layout and spacing standards for UI files.

## Deliverable
\`docs/UI_LAYOUT_STANDARDS.md\`

## Content Required
1. **Standard margin values**
   - Dialog layouts: 9px
   - Nested containers (groupboxes): 6px
   - Rationale for each

2. **Standard spacing values**
   - Vertical spacing between form rows: 6px
   - Horizontal spacing in button groups: 6px
   - Spacing around sections: 12px

3. **When to use stretches vs fixed spacers**
   - Stretches for flexible layouts
   - Fixed spacers for precise alignment

4. **Widget alignment guidelines**
   - Form labels: right-aligned
   - Input fields: left-aligned
   - Buttons: platform conventions

5. **Size policy best practices**
   - Preferred vs Expanding
   - Fixed vs Minimum
   - When to set horizontal/vertical stretch

6. **Examples**
   - XML snippets showing correct patterns
   - Screenshots of good vs bad layouts

## Acceptance Criteria
- [ ] Document created with all required sections
- [ ] Clear, actionable guidelines
- [ ] Visual examples included
- [ ] Reviewed by team

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 6.1

## Estimated Effort
2-3 hours"

gh issue create --title "Standardize dialog margins (Batches 1-7)" --label "P2-medium,ui-consistency,ui/ux,effort-medium" --body "## Description
Standardize layout margins across all 70+ dialogs according to layout standards.

## Standards
- Dialog top-level layouts: 9px margins
- Nested containers (groupboxes, frames): 6px margins
- Consistency within each dialog

## Strategy
- Process in batches of 10 dialogs
- 7 batches total
- Can be combined with other layout improvements

## Changes Required
Set consistent margin values in layout properties:
\`\`\`xml
<property name=\"leftMargin\">
  <number>9</number>
</property>
<property name=\"topMargin\">
  <number>9</number>
</property>
<property name=\"rightMargin\">
  <number>9</number>
</property>
<property name=\"bottomMargin\">
  <number>9</number>
</property>
\`\`\`

## Acceptance Criteria
- [ ] All dialogs use standard margin values
- [ ] Visual consistency across application
- [ ] No visual regressions

## Testing Steps
- Visual inspection of each dialog
- Compare before/after screenshots
- Verify consistent spacing

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 6.2-6.3
- Depends on: Layout standards document

## Estimated Effort
~10 minutes per dialog (12-15 hours total)

## Implementation Notes
Can be split into 7 separate issues for easier tracking"

gh issue create --title "Standardize widget spacing across dialogs" --label "P2-medium,ui-consistency,ui/ux,effort-medium" --body "## Description
Standardize spacing between widgets according to layout standards.

## Changes Required
- Vertical spacing between form rows: 6px
- Horizontal spacing in button groups: 6px
- Replace arbitrary spacer sizes with standard values

## Strategy
- Can be combined with margin standardization
- Process alongside other layout work
- Focus on visual consistency

## Acceptance Criteria
- [ ] Consistent spacing values used throughout
- [ ] Arbitrary spacer sizes replaced with standards
- [ ] Visual harmony in layouts

## Testing Steps
- Visual inspection
- Compare spacing across similar dialogs
- Verify no visual regressions

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 6.4

## Estimated Effort
Can be combined with margin standardization work"

# ============================================================================
# PHASE 7: Dialog Pattern Refactoring (P2)
# ============================================================================

gh issue create --title "Create reusable input dialog base class" --label "P2-medium,ui-consistency,ui/ux,effort-medium" --body "## Description
Create a flexible base dialog class for simple value input, replacing three duplicate dialog implementations.

## Problem
Three dialogs (askOneIntValueDlg, askTwoDoubleValuesDlg, askThreeDoubleValuesDlg) have similar functionality but duplicate code with inconsistencies.

## Solution
Create \`ccSimpleInputDialog\` that can display N labeled inputs dynamically.

## Affected Files
- New: \`qCC/ccSimpleInputDialog.h\`
- New: \`qCC/ccSimpleInputDialog.cpp\`
- New: \`qCC/ui_templates/simpleInputDialog.ui\`
- Refactor: \`askOneIntValueDlg.{h,cpp}\`
- Refactor: \`askTwoDoubleValuesDlg.{h,cpp}\`
- Refactor: \`askThreeDoubleValuesDlg.{h,cpp}\`

## Features
- Support for int/double/string inputs
- Configurable labels
- Validation and range support
- Consistent appearance and behavior

## Example Usage
\`\`\`cpp
ccSimpleInputDialog dlg(this);
dlg.setWindowTitle(\"Enter Values\");
dlg.addDoubleInput(\"X\", 0.0, -100.0, 100.0);
dlg.addDoubleInput(\"Y\", 0.0, -100.0, 100.0);
dlg.addIntInput(\"Count\", 1, 1, 1000);
if (dlg.exec()) {
    double x = dlg.getDoubleValue(0);
    double y = dlg.getDoubleValue(1);
    int count = dlg.getIntValue(2);
}
\`\`\`

## Benefits
- Less duplicate code
- Easier to maintain
- More consistent behavior
- Easier to create new input dialogs

## Acceptance Criteria
- [ ] ccSimpleInputDialog class created
- [ ] Supports int, double, and string inputs
- [ ] All existing use cases work with new class
- [ ] No behavioral regressions
- [ ] Code is cleaner and more maintainable

## Testing Steps
1. Find all uses of askOneIntValueDlg, askTwoDoubleValuesDlg, askThreeDoubleValuesDlg
2. Test each use case with new dialog
3. Verify validation and ranges work
4. Verify appearance matches standards

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 7.1

## Estimated Effort
4-6 hours"

gh issue create --title "Audit and standardize progress dialog patterns" --label "P2-medium,ui-consistency,ui/ux,effort-small" --body "## Description
Find and standardize all progress dialog implementations for consistent appearance and behavior.

## Tasks
1. Find all progress dialog implementations
2. Document current patterns
3. Create standard progress dialog wrapper if needed
4. Ensure consistent appearance across application

## Acceptance Criteria
- [ ] All progress dialogs identified
- [ ] Consistent appearance and behavior
- [ ] Standard pattern documented
- [ ] Consider QProgressDialog best practices

## Testing Steps
1. Run operations that show progress
2. Verify consistent appearance
3. Test cancel functionality
4. Test progress updates

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 7.2

## Estimated Effort
2-4 hours"

gh issue create --title "Audit and standardize file selection patterns" --label "P2-medium,ui-consistency,ui/ux,effort-small" --body "## Description
Ensure all file selection patterns use QFileDialog consistently with standard \"Browse\" button behavior.

## Tasks
1. Find all file selection implementations
2. Ensure QFileDialog used consistently
3. Standardize \"Browse\" button placement
4. Ensure file path validation is consistent

## Acceptance Criteria
- [ ] All file selection uses QFileDialog
- [ ] Consistent browse button behavior
- [ ] Consistent path validation
- [ ] Consistent appearance

## Testing Steps
1. Test file selection in various dialogs
2. Verify browse button behavior
3. Test path validation
4. Test keyboard navigation to browse button

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 7.3

## Estimated Effort
2-3 hours"

# ============================================================================
# PHASE 8: Widget Naming Conventions (P3)
# ============================================================================

gh issue create --title "Create widget naming standards document" --label "P3-low,ui-documentation,ui/ux,effort-small,good-first-issue" --body "## Description
Document standard naming conventions for widgets in UI files.

## Deliverable
\`docs/UI_NAMING_CONVENTIONS.md\`

## Content Required
1. **Naming format**: \`[purpose][WidgetType]\`
   - Examples: \`radiusDoubleSpinBox\`, \`okPushButton\`, \`optionsGroupBox\`

2. **Widget type suffixes**
   - QPushButton: \`PushButton\`
   - QLineEdit: \`LineEdit\`
   - QSpinBox: \`SpinBox\`
   - QDoubleSpinBox: \`DoubleSpinBox\`
   - QComboBox: \`ComboBox\`
   - QCheckBox: \`CheckBox\`
   - QRadioButton: \`RadioButton\`
   - QLabel: \`Label\`
   - QGroupBox: \`GroupBox\`
   - etc.

3. **Purpose naming**
   - Use camelCase
   - Be descriptive but concise
   - Use domain terminology

4. **When to abbreviate vs full names**

5. **Examples for common patterns**

## Application Strategy
- Apply to new dialogs only (don't rename existing)
- Document rationale

## Acceptance Criteria
- [ ] Document created
- [ ] Clear, actionable guidelines
- [ ] 20+ examples included

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 8.1

## Estimated Effort
1-2 hours"

# ============================================================================
# PHASE 9: Label Capitalization (P3)
# ============================================================================

gh issue create --title "Document capitalization rules and fix opportunistically" --label "P3-low,ui-consistency,ui/ux,effort-small" --body "## Description
Document capitalization rules for labels, buttons, and titles. Fix capitalization opportunistically when editing dialogs.

## Rules to Document
1. **Sentence case for labels**: \"Maximum distance\" (not \"Maximum Distance\")
2. **Title case for dialog titles**: \"Export Coordinates to Scalar Field\"
3. **Sentence case for buttons**: Exception for standard \"OK\"/\"Cancel\"
4. **ALL CAPS only for standard acronyms**: \"CPU\", \"GPU\", \"RGB\"

## Strategy
- Don't make standalone PRs just for capitalization
- Fix opportunistically when editing dialogs for other reasons
- Low priority cleanup

## Acceptance Criteria
- [ ] Capitalization rules documented
- [ ] Examples provided
- [ ] Gradual improvement over time

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 9

## Estimated Effort
1 hour (documentation only)"

# ============================================================================
# PHASE 10: Style Guide and Documentation (P1)
# ============================================================================

gh issue create --title "Create comprehensive UI style guide" --label "P1-high,ui-documentation,ui/ux,effort-medium" --body "## Description
Create a comprehensive UI style guide consolidating all UI/UX standards into one reference document.

## Deliverable
\`docs/UI_STYLE_GUIDE.md\`

## Content Required
Consolidate and expand on:
1. Layout standards (margins, spacing)
2. Color usage (palette roles, semantic colors)
3. Typography (capitalization, terminology)
4. Widget naming conventions
5. Accessibility requirements
   - Tooltips
   - Tab order
   - Keyboard shortcuts
   - Label buddies
6. Dialog button patterns
7. Icon usage
8. Responsive design principles
9. Theme compatibility
10. Examples and anti-patterns
11. Screenshots of good vs bad examples

## Structure
- Quick reference section
- Detailed guidelines for each topic
- Code examples
- Visual examples
- Checklist for new dialogs

## Acceptance Criteria
- [ ] Comprehensive document created
- [ ] All standards consolidated
- [ ] 20+ code examples
- [ ] 10+ visual examples (screenshots)
- [ ] Reviewed by team
- [ ] Integrated into developer documentation

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 10.1
- Consolidates: Tooltip guidelines, Layout standards, Naming conventions

## Estimated Effort
6-8 hours"

gh issue create --title "Create dialog template files" --label "P2-medium,ui-documentation,ui/ux,effort-small,good-first-issue" --body "## Description
Create ready-to-use dialog templates following all UI standards.

## Deliverables
- \`qCC/ui_templates/_templates/simple_dialog_template.ui\`
- \`qCC/ui_templates/_templates/input_dialog_template.ui\`
- \`qCC/ui_templates/_templates/tabbed_dialog_template.ui\`
- \`qCC/ui_templates/_templates/README.md\` (usage instructions)

## Template Features
- Properly configured margins and spacing
- QDialogButtonBox included
- Commented sections showing where to add content
- Example tab order
- Example tooltips
- Example mnemonics
- Example label buddies
- Follows all standards from style guide

## Usage
Developers can copy template and customize for new dialogs

## Acceptance Criteria
- [ ] 3-5 templates created
- [ ] All templates follow standards
- [ ] Usage instructions provided
- [ ] Templates tested by creating new dialog from each

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 10.2
- Depends on: UI Style Guide

## Estimated Effort
2-3 hours"

# ============================================================================
# PHASE 11: Automated Quality Checks (P2)
# ============================================================================

gh issue create --title "Create UI linting script for quality enforcement" --label "P2-medium,ui-automation,ui/ux,effort-medium" --body "## Description
Create a Python script to automatically check UI files for common issues and standard violations.

## Deliverable
\`scripts/lint_ui_files.py\`

## Checks to Implement
1. **Critical (fail CI)**:
   - Hardcoded colors in styleSheet properties
   - Missing QDialogButtonBox in dialogs

2. **Warnings**:
   - Hardcoded geometry in dialogs
   - Missing tab stops
   - Missing tooltips on buttons/inputs
   - Inconsistent margin values
   - Manual buttons instead of QDialogButtonBox
   - Missing label buddies
   - Duplicate mnemonics within dialog

## Output Format
\`\`\`
Checking qCC/ui_templates/someDlg.ui...
  ERROR: Hardcoded color at line 226: 'color: red;'
  WARNING: Missing <tabstops> section
  WARNING: 3 widgets missing tooltips

Summary:
  Files checked: 100
  Errors: 5
  Warnings: 47
\`\`\`

## Implementation
- Parse .ui files (XML)
- Check each rule
- Generate report
- Exit code: 0 if no errors, 1 if errors found

## Acceptance Criteria
- [ ] Script created and executable
- [ ] All checks implemented
- [ ] Clear, actionable output
- [ ] Fast execution (< 30 seconds for all files)
- [ ] Can run on specific files or entire project
- [ ] Tested against known good and bad files

## Usage
\`\`\`bash
# Check all files
python scripts/lint_ui_files.py

# Check specific files
python scripts/lint_ui_files.py qCC/ui_templates/*.ui

# Generate report
python scripts/lint_ui_files.py --report report.html
\`\`\`

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 11.1

## Estimated Effort
6-8 hours"

gh issue create --title "Integrate UI linting into CI/CD pipeline" --label "P2-medium,ui-automation,ui/ux,effort-small" --body "## Description
Add UI linting to GitHub Actions CI pipeline to enforce standards on all PRs.

## Changes Required
Update \`.github/workflows/\` to include:
\`\`\`yaml
- name: Lint UI files
  run: python scripts/lint_ui_files.py
  continue-on-error: true  # Initially warn only
\`\`\`

## Enforcement Strategy
1. **Phase 1**: Warnings only (don't fail builds)
2. **Phase 2**: Fail on critical violations (hardcoded colors)
3. **Phase 3**: Fail on all violations for new/modified files
4. **Phase 4**: Full enforcement after cleanup complete

## Acceptance Criteria
- [ ] UI linting runs on all PRs
- [ ] Results visible in PR checks
- [ ] Clear error messages for violations
- [ ] Documentation for developers on how to fix issues

## References
- See \`UI_MODERNIZATION_PLAN.md\` Phase 11.2
- Depends on: UI linting script

## Estimated Effort
2-3 hours"

# ============================================================================
# SUMMARY TRACKING ISSUE
# ============================================================================

gh issue create --title "[TRACKER] CloudCompare UI Modernization Initiative" --label "P0-critical,ui/ux,ui-documentation" --body "## Overview
This is the master tracking issue for the CloudCompare UI Modernization Initiative. Based on a comprehensive audit of 100+ UI files, we've identified 60+ atomic improvements to enhance consistency, accessibility, and user experience.

## Documentation
- **Master Plan**: \`UI_MODERNIZATION_PLAN.md\`
- **Audit Report**: See plan for detailed findings

## Key Statistics
- **Total Issues**: 60+ atomic tasks
- **Files Affected**: 100+ UI files
- **Estimated Total Effort**: ~100-120 hours
- **Priority Breakdown**:
  - P0 (Critical): 8 issues (~15 hours)
  - P1 (High): 25 issues (~60 hours)
  - P2 (Medium): 20 issues (~35 hours)
  - P3 (Low): 7 issues (~10 hours)

## Progress Tracking

### Phase 1: Critical Fixes - Hardcoded Colors (P0)
- [ ] #<issue> Remove hardcoded colors - Batch 1/4 (core dialogs)
- [ ] #<issue> Remove hardcoded colors - Batch 2/4 (IO dialogs)
- [ ] #<issue> Remove hardcoded colors - Batch 3/4 (plugin dialogs)
- [ ] #<issue> Remove hardcoded colors - Batch 4/4 (remaining)

### Phase 2: Dialog Button Standardization (P0-P1)
- [ ] #<issue> Convert to QDialogButtonBox - Batch 1/3 (core)
- [ ] #<issue> Convert to QDialogButtonBox - Batch 2/3 (IO)
- [ ] #<issue> Convert to QDialogButtonBox - Batch 3/3 (plugins)

### Phase 3: Responsive Design (P1)
- [ ] #<issue> Remove hardcoded geometry - Batch 1/4 (simple)
- [ ] #<issue> Remove hardcoded geometry - Batch 2/4 (medium)
- [ ] #<issue> Remove hardcoded geometry - Batch 3/4 (complex)
- [ ] #<issue> Remove hardcoded geometry - Batch 4/4 (plugins)
- [ ] #<issue> Remove unnecessary min/max sizes

### Phase 4: Accessibility Improvements (P1)
- [ ] #<issue> Add tab order - Batch 1/6 (core)
- [ ] #<issue> Add tab order - Batch 2/6 (IO)
- [ ] #<issue> Add tab order - Batches 3-6 (remaining)
- [ ] #<issue> Add label buddies - Batch 1/5 (core)
- [ ] #<issue> Add label buddies - Batches 2-5 (remaining)
- [ ] #<issue> Add keyboard shortcuts - Batch 1/5 (core)
- [ ] #<issue> Add keyboard shortcuts - Batches 2-5 (remaining)

### Phase 5: Tooltip Enhancement (P1-P2)
- [ ] #<issue> Create tooltip guidelines
- [ ] #<issue> Add tooltips to main window
- [ ] #<issue> Add tooltips to core dialogs (Batches 1-2)
- [ ] #<issue> Add tooltips to remaining dialogs (Batches 3-8)

### Phase 6: Layout Consistency (P2)
- [ ] #<issue> Create layout standards document
- [ ] #<issue> Standardize dialog margins (Batches 1-7)
- [ ] #<issue> Standardize widget spacing

### Phase 7: Dialog Pattern Refactoring (P2)
- [ ] #<issue> Create reusable input dialog base class
- [ ] #<issue> Standardize progress dialogs
- [ ] #<issue> Standardize file selection patterns

### Phase 8: Widget Naming Conventions (P3)
- [ ] #<issue> Create naming standards document

### Phase 9: Label Capitalization (P3)
- [ ] #<issue> Document capitalization rules

### Phase 10: Documentation (P1-P2)
- [ ] #<issue> Create comprehensive UI style guide
- [ ] #<issue> Create dialog template files

### Phase 11: Automation (P2)
- [ ] #<issue> Create UI linting script
- [ ] #<issue> Integrate linting into CI

## Quick Wins (Start Here!)
For immediate impact, start with these:
1. Remove hardcoded colors from 5 worst offenders (~2.5 hours)
2. Add tooltips to main window (~1 hour)
3. Convert 3 core dialogs to QDialogButtonBox (~2 hours)

## Success Metrics
- [ ] 100% theme compatibility (no hardcoded colors)
- [ ] 100% platform-native button ordering (QDialogButtonBox)
- [ ] 100% keyboard navigation (tab stops)
- [ ] 90%+ tooltip coverage
- [ ] 0 hardcoded geometry
- [ ] Consistent layout margins
- [ ] CI enforcement of standards

## Contributing
Each issue is designed to be:
- **Atomic**: Can be completed independently
- **Small**: 1-3 hours of focused work
- **Testable**: Clear acceptance criteria
- **Mergeable**: Can be reviewed and merged quickly

Issues labeled \`good-first-issue\` are great entry points for new contributors.

## Labels Used
- **Priority**: P0-critical, P1-high, P2-medium, P3-low
- **Category**: ui-theming, ui-accessibility, ui-responsive, ui-consistency, ui-documentation, ui-automation
- **Effort**: effort-small, effort-medium
- **Entry**: good-first-issue

## Timeline
See \`UI_MODERNIZATION_PLAN.md\` for detailed implementation timeline and strategy.

## Questions?
Post questions in this issue or refer to the documentation in \`UI_MODERNIZATION_PLAN.md\`.

---
**Note**: Issue numbers will be filled in as issues are created. This tracker will be updated to link all related issues."

echo ""
echo "✓ All issues created successfully!"
echo ""
echo "Summary:"
echo "  - Labels created: 14"
echo "  - Issues created: 40+"
echo "  - Master tracker issue created"
echo ""
echo "Next steps:"
echo "1. Review and update the tracker issue with actual issue numbers"
echo "2. Start with P0-critical issues (hardcoded colors)"
echo "3. Consider assigning issues to team members"
echo "4. Update UI_MODERNIZATION_PLAN.md with issue references"
