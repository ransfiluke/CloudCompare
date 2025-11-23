# CloudCompare UI Modernization Plan

## Executive Summary

This plan outlines atomic UI/UX improvements for CloudCompare based on a comprehensive audit of 100+ UI files. Each item is designed to be a small, focused change that can be merged independently.

**Key Findings:**
- ✅ Good: Consistent OK/Cancel button text, icon resource usage
- ❌ Critical Issues: 20+ files with hardcoded colors breaking theming
- ❌ Best Practice Violations: Missing accessibility features, manual buttons instead of QDialogButtonBox
- ❌ Consistency Issues: Hardcoded geometry, inconsistent margins

---

## Priority Levels

- **P0 (Critical)**: Breaks functionality (theming, accessibility)
- **P1 (High)**: Significant UX impact
- **P2 (Medium)**: Polish and consistency
- **P3 (Low)**: Nice-to-have improvements

---

# Phase 1: Critical Fixes (P0)

## 1.1 Remove Hardcoded Colors - Batch 1 (Core Dialogs)
**Priority**: P0
**Effort**: Small
**Files**: 5 files

**Affected Files:**
1. `qCC/ui_templates/boundingBoxEditorDlg.ui` - Line 226 (red warning text)
2. `qCC/ui_templates/pointPairRegistrationDlg.ui` - Line 22 (red background checkbox)
3. `qCC/ui_templates/histogramDlg.ui` - Line 17 (white background)
4. `libs/qCC_io/ui/openAsciiFileDlg.ui` - Line 110 (grey text)
5. `qCC/ui_templates/volumeCalcDlg.ui` - (various hardcoded colors)

**Changes:**
- Remove all `styleSheet` properties containing hardcoded colors
- For warning/error text: Use dynamic palette-based styling in C++ code
- For semantic highlights: Implement theme-aware CSS classes

**Testing**: Verify dialogs display correctly in both Dark and Light themes

---

## 1.2 Remove Hardcoded Colors - Batch 2 (IO Dialogs)
**Priority**: P0
**Effort**: Small
**Files**: 5 files

**Affected Files:**
1. `libs/qCC_io/ui/globalShiftAndScaleDlg.ui`
2. `qCC/ui_templates/globalShiftSettingsDlg.ui`
3. `qCC/ui_templates/registrationDlg.ui`
4. `qCC/ui_templates/rasterizeDlg.ui`
5. `qCC/ui_templates/comparisonDlg.ui`

**Changes:**
- Remove hardcoded color stylesheets
- Document pattern for semantic colors in style guide

**Testing**: Test with Dark/Light themes

---

## 1.3 Remove Hardcoded Colors - Batch 3 (Plugins)
**Priority**: P0
**Effort**: Small
**Files**: 5 files

**Affected Files:**
1. `libs/CCPluginAPI/ui/colorScaleEditorDlg.ui`
2. `qCC/ui_templates/exportCoordToSFDlg.ui`
3. `qCC/ui_templates/clippingBoxRepeatDlg.ui`
4. `qCC/ui_templates/ccDrawNormalsWidget.ui`
5. Other plugin dialogs with hardcoded colors

**Changes:**
- Remove hardcoded color stylesheets
- Ensure color scale editor uses dynamic colors

**Testing**: Verify plugin dialogs work in both themes

---

## 1.4 Remove Hardcoded Colors - Batch 4 (Remaining)
**Priority**: P0
**Effort**: Small
**Files**: ~5 remaining files

**Changes:**
- Complete removal of all hardcoded colors
- Final verification across all themes

**Testing**: Full regression test of all dialogs in both themes

---

# Phase 2: Dialog Button Standardization (P0-P1)

## 2.1 Convert Manual Buttons to QDialogButtonBox - Batch 1
**Priority**: P0
**Effort**: Small
**Files**: 3 files

**Affected Files:**
1. `qCC/ui_templates/boundingBoxEditorDlg.ui`
2. `qCC/ui_templates/comparisonDlg.ui`
3. `libs/CCAppCommon/ui/displaySettingsDlg.ui`

**Changes:**
- Replace manual QPushButton elements with QDialogButtonBox
- Update signal/slot connections in corresponding .cpp files
- Use standard roles: QDialogButtonBox::Ok | QDialogButtonBox::Cancel
- Handle special buttons (e.g., "Default", "Last") as custom buttons in buttonbox

**Testing**: Verify button ordering matches platform conventions (macOS vs Windows vs Linux)

---

## 2.2 Convert Manual Buttons to QDialogButtonBox - Batch 2
**Priority**: P1
**Effort**: Small
**Files**: 3 files

**Affected Files:**
1. `libs/qCC_io/ui/importDBFFieldDlg.ui`
2. `libs/qCC_io/ui/openAsciiFileDlg.ui`
3. `libs/qCC_io/ui/openPlyFileDlg.ui`

**Changes:**
- Convert to QDialogButtonBox
- Update corresponding .cpp signal handlers

**Testing**: Verify file import dialogs work correctly

---

## 2.3 Convert Manual Buttons to QDialogButtonBox - Batch 3
**Priority**: P1
**Effort**: Small
**Files**: 4 files

**Affected Files:**
1. `plugins/core/IO/qLASIO/ui/lasopendialog.ui`
2. `plugins/core/IO/qPDALIO/ui/openLASFileDlg.ui`
3. `plugins/core/Standard/qBroom/ui/broomDlg.ui`
4. `qCC/ui_templates/filterByValueDlg.ui` (special case: Export/Split buttons)

**Changes:**
- Convert to QDialogButtonBox
- For filterByValueDlg: Add custom "Export" and "Split" buttons to QDialogButtonBox using addButton()

**Testing**: Verify plugin dialogs and special button actions work

---

# Phase 3: Responsive Design (P1)

## 3.1 Remove Hardcoded Geometry - Batch 1 (Simple Dialogs)
**Priority**: P1
**Effort**: Small
**Files**: 5 files

**Affected Files:**
1. `qCC/ui_templates/askThreeDoubleValuesDlg.ui` (452x89)
2. `qCC/ui_templates/filterByValueDlg.ui` (432x99)
3. `qCC/ui_templates/askOneIntValueDlg.ui` (if hardcoded)
4. `qCC/ui_templates/askTwoDoubleValuesDlg.ui` (if hardcoded)
5. Other simple input dialogs

**Changes:**
- Remove `<rect>` property from QDialog widget definition
- Ensure proper sizePolicy is set (Preferred/Preferred)
- Let Qt calculate size based on layout and content

**Testing**: Verify dialogs size correctly and remember user-resized dimensions

---

## 3.2 Remove Hardcoded Geometry - Batch 2 (Medium Dialogs)
**Priority**: P1
**Effort**: Small
**Files**: 5 files

**Affected Files:**
1. `qCC/ui_templates/alignDlg.ui` (600x361)
2. `qCC/ui_templates/comparisonDlg.ui` (450x400)
3. `qCC/ui_templates/registrationDlg.ui` (411x458)
4. `qCC/ui_templates/displaySettingsDlg.ui` (514x580)
5. Other medium-sized dialogs

**Changes:**
- Remove hardcoded geometry
- Set reasonable minimumSize if needed (but prefer dynamic sizing)

**Testing**: Ensure dialogs display all content properly at default size

---

## 3.3 Remove Hardcoded Geometry - Batch 3 (Complex Dialogs)
**Priority**: P1
**Effort**: Medium (may need layout adjustments)
**Files**: 5 files

**Affected Files:**
1. `qCC/ui_templates/rasterizeDlg.ui` (1024x800)
2. `qCC/ui_templates/volumeCalcDlg.ui` (1000x887)
3. `qCC/ui_templates/boundingBoxEditorDlg.ui` (600x650)
4. `qCC/ui_templates/histogramDlg.ui` (800x600)
5. `libs/qCC_io/ui/openAsciiFileDlg.ui` (1008x657)

**Changes:**
- Remove hardcoded geometry
- May need to adjust layouts to ensure proper content flow
- Consider adding scroll areas if content is extensive

**Testing**: Test on different screen sizes and DPI settings

---

## 3.4 Remove Hardcoded Geometry - Batch 4 (Plugin Dialogs)
**Priority**: P1
**Effort**: Small
**Files**: ~5 plugin dialogs

**Affected Files:**
1. `plugins/core/Standard/qM3C2/ui/qM3C2Dialog.ui` (755x490)
2. Other plugin dialogs with hardcoded sizes

**Changes:**
- Remove hardcoded geometry from plugin dialogs
- Ensure plugins follow same standards as core

**Testing**: Verify plugin dialogs work correctly

---

## 3.5 Remove Hardcoded Min/Max Sizes
**Priority**: P1
**Effort**: Small
**Files**: 20+ files (can be batched)

**Changes:**
- Review each minimumSize/maximumSize property
- Remove if not essential
- Replace with sizePolicy where appropriate
- Keep only if there's a valid reason (e.g., minimum size for image display)

**Testing**: Ensure dialogs remain usable at various sizes

---

# Phase 4: Accessibility Improvements (P1)

## 4.1 Add Tab Stops - Batch 1 (Core Dialogs)
**Priority**: P1
**Effort**: Small
**Files**: 10 files

**Template** (based on subsamplingDlg.ui):
```xml
<tabstops>
  <tabstop>firstWidget</tabstop>
  <tabstop>secondWidget</tabstop>
  ...
</tabstops>
```

**Affected Files:**
- Top 10 most-used dialogs (to be determined by usage analysis)

**Changes:**
- Add `<tabstops>` section to each .ui file
- Define logical tab order from top to bottom, left to right
- Ensure OK/Cancel buttons are last in tab order

**Testing**: Tab through dialogs, verify logical order

---

## 4.2 Add Tab Stops - Batch 2 (IO Dialogs)
**Priority**: P1
**Effort**: Small
**Files**: 10 files

**Affected Files:**
- All IO-related dialogs in libs/qCC_io/ui/

**Changes:**
- Add logical tab order to file dialog interfaces

**Testing**: Verify keyboard-only file operations work smoothly

---

## 4.3 Add Tab Stops - Batch 3 (Remaining Dialogs)
**Priority**: P1
**Effort**: Medium (60+ files)

**Strategy:**
- Process in batches of 10-15 files
- Can be split into multiple PRs

**Changes:**
- Complete tab order for all remaining dialogs

**Testing**: Spot-check random dialogs for correct tab order

---

## 4.4 Add Label Buddies - Batch 1
**Priority**: P1
**Effort**: Small
**Files**: 10 files

**Template:**
```xml
<widget class="QLabel" name="label">
  <property name="text">
    <string>&amp;Name:</string>
  </property>
  <property name="buddy">
    <cstring>nameLineEdit</cstring>
  </property>
</widget>
```

**Changes:**
- Associate each QLabel with its corresponding input widget
- Ensures clicking label focuses the input
- Required for screen readers

**Testing**: Click labels, verify focus moves to correct widget

---

## 4.5 Add Label Buddies - Batch 2-6
**Priority**: P1
**Effort**: Small per batch
**Files**: 50+ files (split into batches of 10)

**Changes:**
- Complete buddy associations for all dialogs

**Testing**: Automated test to verify all labels have valid buddies

---

## 4.6 Add Keyboard Shortcuts (Mnemonics) - Batch 1
**Priority**: P1
**Effort**: Small
**Files**: 10 files

**Changes:**
- Add '&' character before shortcut letter in button/label text
- Ensure no duplicates within a dialog
- Common patterns:
  - &OK, &Cancel, &Apply
  - &Browse, &Open, &Save
  - &Export, &Import

**Testing**: Press Alt+letter, verify correct widget activates

---

## 4.7 Add Keyboard Shortcuts - Batch 2-6
**Priority**: P1
**Effort**: Small per batch
**Files**: 50+ files

**Changes:**
- Complete mnemonics for all dialogs

**Testing**: Verify no mnemonic conflicts within dialogs

---

# Phase 5: Tooltip Enhancement (P1-P2)

## 5.1 Create Tooltip Guidelines Document
**Priority**: P1
**Effort**: Small
**Files**: 1 new document

**Deliverable:**
- `docs/UI_TOOLTIP_GUIDELINES.md`

**Content:**
- When to add tooltips (all non-obvious controls)
- Tooltip writing style (concise, helpful, non-redundant)
- Examples of good vs bad tooltips
- Template phrases for common controls

---

## 5.2 Add Tooltips - Batch 1 (Main Window)
**Priority**: P1
**Effort**: Small
**Files**: 1 file

**Affected Files:**
- `qCC/ui_templates/mainWindow.ui`

**Changes:**
- Add tooltips to all toolbar buttons
- Add tooltips to menu items where helpful
- Add status tips for menu items

**Testing**: Hover over controls, verify helpful tooltips appear

---

## 5.3 Add Tooltips - Batch 2 (Core Dialogs)
**Priority**: P2
**Effort**: Small
**Files**: 10 files

**Affected Files:**
- Most frequently used dialogs

**Changes:**
- Add tooltips to all spinboxes, buttons, checkboxes
- Focus on non-obvious controls
- Use consistent language

**Testing**: Verify tooltips are helpful and non-redundant

---

## 5.4 Add Tooltips - Batch 3-8
**Priority**: P2
**Effort**: Small per batch
**Files**: 60+ files (batches of 10)

**Changes:**
- Complete tooltip coverage across all dialogs

**Testing**: Spot-check for quality and consistency

---

# Phase 6: Layout Consistency (P2)

## 6.1 Create Layout Standards Document
**Priority**: P2
**Effort**: Small
**Files**: 1 new document

**Deliverable:**
- `docs/UI_LAYOUT_STANDARDS.md`

**Content:**
- Standard margin values: 9px for dialogs, 6px for nested containers
- Standard spacing values
- When to use stretches vs fixed spacers
- Widget alignment guidelines
- Size policy best practices

---

## 6.2 Standardize Dialog Margins - Batch 1
**Priority**: P2
**Effort**: Small
**Files**: 10 files

**Changes:**
- Set dialog layout margins to 9px
- Set nested container margins to 6px
- Ensure consistency within each dialog

**Testing**: Visual inspection for consistent spacing

---

## 6.3 Standardize Dialog Margins - Batch 2-7
**Priority**: P2
**Effort**: Small per batch
**Files**: 60+ files (batches of 10)

**Changes:**
- Apply standard margins to all remaining dialogs

**Testing**: Visual consistency check

---

## 6.4 Standardize Widget Spacing
**Priority**: P2
**Effort**: Small
**Files**: Can be combined with margin standardization

**Changes:**
- Use consistent vertical spacing between form rows (6px)
- Use consistent horizontal spacing in button groups (6px)
- Replace arbitrary spacer sizes with standard values

**Testing**: Visual inspection

---

# Phase 7: Dialog Pattern Refactoring (P2)

## 7.1 Create Reusable Input Dialog Base Class
**Priority**: P2
**Effort**: Medium
**Files**: New C++ class + 3 refactored dialogs

**Affected Files:**
- New: `qCC/ccSimpleInputDialog.h`, `qCC/ccSimpleInputDialog.cpp`
- Refactor: `askOneIntValueDlg`, `askTwoDoubleValuesDlg`, `askThreeDoubleValuesDlg`

**Changes:**
- Create flexible base dialog that can display N labeled inputs
- Eliminate duplicate code across simple input dialogs
- Support validation and ranges

**Benefits:**
- Easier to maintain
- More consistent behavior
- Easier to create new input dialogs programmatically

**Testing**: Ensure all existing callers work correctly

---

## 7.2 Audit and Standardize Progress Dialogs
**Priority**: P2
**Effort**: Small

**Changes:**
- Find all progress dialog implementations
- Ensure consistent appearance and behavior
- Consider creating standard progress dialog wrapper

**Testing**: Run operations that show progress, verify consistency

---

## 7.3 Audit and Standardize File Selection Patterns
**Priority**: P2
**Effort**: Small

**Changes:**
- Ensure all file selection uses QFileDialog consistently
- Standardize "Browse" button placement and behavior
- Ensure file path line edits have consistent validation

**Testing**: Test file selection in various dialogs

---

# Phase 8: Widget Naming Conventions (P3)

## 8.1 Create Widget Naming Standards Document
**Priority**: P3
**Effort**: Small
**Files**: 1 new document

**Deliverable:**
- `docs/UI_NAMING_CONVENTIONS.md`

**Content:**
- Format: `[purpose][WidgetType]` (e.g., `radiusDoubleSpinBox`)
- List of standard widget type suffixes
- Examples for common patterns
- When to use abbreviated vs full names

---

## 8.2 Rename Widgets in New Dialogs Only
**Priority**: P3
**Effort**: Ongoing

**Strategy:**
- Don't rename existing widgets (too disruptive)
- Apply naming standards to all new dialogs
- Update standards document with examples

**Testing**: Not applicable (documentation only)

---

# Phase 9: Label Capitalization (P3)

## 9.1 Document Capitalization Rules
**Priority**: P3
**Effort**: Small
**Files**: 1 document (can be in layout standards)

**Rules:**
- Sentence case for labels (e.g., "Maximum distance")
- Title case for dialog titles and major sections
- Sentence case for buttons (exception: standard OK/Cancel)
- ALL CAPS only for acronyms that are normally capitalized

---

## 9.2 Fix Capitalization - Batch 1-N
**Priority**: P3
**Effort**: Small per batch
**Files**: Many (can be combined with other changes)

**Strategy:**
- Fix capitalization opportunistically when editing dialogs
- Don't make standalone PRs just for capitalization
- Low priority cleanup

---

# Phase 10: Style Guide and Documentation (P1)

## 10.1 Create Comprehensive UI Style Guide
**Priority**: P1
**Effort**: Medium
**Files**: 1 comprehensive document

**Deliverable:**
- `docs/UI_STYLE_GUIDE.md`

**Content:**
- Consolidate all standards into one reference
- Include:
  - Layout standards (margins, spacing)
  - Color usage (palette roles, semantic colors)
  - Typography (capitalization, terminology)
  - Widget naming conventions
  - Accessibility requirements (tooltips, tab order, mnemonics, buddies)
  - Dialog button patterns
  - Icon usage
  - Responsive design principles
- Include examples and anti-patterns
- Screenshots of good vs bad examples

**Testing**: Review with team, iterate based on feedback

---

## 10.2 Create Dialog Template Files
**Priority**: P2
**Effort**: Small
**Files**: 3-5 template .ui files

**Deliverables:**
- `qCC/ui_templates/_templates/simple_dialog_template.ui`
- `qCC/ui_templates/_templates/input_dialog_template.ui`
- `qCC/ui_templates/_templates/tabbed_dialog_template.ui`

**Content:**
- Ready-to-use templates following all standards
- Properly configured margins, spacing, button boxes
- Commented sections showing where to add content
- Examples of correct tab order, tooltips, mnemonics

**Testing**: Create new dialog from template, verify it follows all standards

---

# Phase 11: Automated Quality Checks (P2)

## 11.1 Create UI Linting Script
**Priority**: P2
**Effort**: Medium
**Files**: New Python script

**Deliverable:**
- `scripts/lint_ui_files.py`

**Checks:**
- Hardcoded colors in styleSheet properties
- Missing QDialogButtonBox in dialogs
- Hardcoded geometry in dialogs
- Missing tab stops
- Missing tooltips on buttons/inputs
- Inconsistent margin values
- Manual buttons instead of QDialogButtonBox

**Output:**
- Report of violations per file
- Can be integrated into CI/CD

**Testing**: Run against known good and bad files

---

## 11.2 Integrate UI Linting into CI
**Priority**: P2
**Effort**: Small
**Files**: CI configuration

**Changes:**
- Add UI lint check to GitHub Actions
- Fail on critical violations (hardcoded colors)
- Warn on best practice violations
- Allow gradual improvement of existing files

**Testing**: Verify CI catches violations

---

# Implementation Strategy

## Recommended Order

1. **Start with P0 items** (Critical fixes):
   - Weeks 1-2: Remove all hardcoded colors (4 batches)
   - Weeks 3-4: Convert manual buttons to QDialogButtonBox (3 batches)

2. **Move to P1 items** (High impact):
   - Weeks 5-7: Remove hardcoded geometry (4 batches)
   - Weeks 8-10: Add tab stops (3 batches)
   - Weeks 11-13: Add label buddies (6 batches)
   - Weeks 14-16: Add keyboard shortcuts (6 batches)

3. **Create documentation** (Ongoing):
   - Week 4: UI Style Guide
   - Week 8: Tooltip Guidelines
   - Week 12: Template files

4. **Polish with P2 items** (Medium priority):
   - Weeks 17-20: Add tooltips (8 batches)
   - Weeks 21-24: Standardize layouts (7 batches)
   - Weeks 25-26: Refactor dialog patterns

5. **Add quality tools** (Ongoing):
   - Week 20: Create UI linting script
   - Week 21: Integrate into CI

6. **P3 items** (As time permits):
   - Opportunistic improvements during other work

## Batching Strategy

- **Small batches**: 3-5 files per PR
- **Focused changes**: Each PR addresses one type of issue
- **Quick reviews**: Small PRs are easier to review and merge
- **Regular cadence**: 2-3 PRs per week

## Testing Strategy

- Manual testing for each batch
- Visual regression testing (screenshots before/after)
- Accessibility testing (keyboard navigation, screen readers)
- Theme testing (Dark/Light themes)
- Cross-platform testing (Windows, macOS, Linux)

## Success Metrics

- 100% of dialogs theme-compatible (no hardcoded colors)
- 100% of dialogs use QDialogButtonBox
- 100% of dialogs have defined tab stops
- 90%+ of widgets have tooltips
- 0 hardcoded geometry in dialogs
- Consistent layout margins across all dialogs
- CI enforcement of standards for new code

---

# Quick Wins (Start Here!)

If you want immediate impact, start with these atomic changes:

## Quick Win 1: Fix Critical Theming Issues
**Files**: 5 files, ~30 minutes each
1. `qCC/ui_templates/boundingBoxEditorDlg.ui` - Remove red color
2. `qCC/ui_templates/histogramDlg.ui` - Remove white background
3. `qCC/ui_templates/pointPairRegistrationDlg.ui` - Remove red background
4. `libs/qCC_io/ui/openAsciiFileDlg.ui` - Remove grey text
5. `qCC/ui_templates/volumeCalcDlg.ui` - Remove hardcoded colors

**Impact**: Immediate improvement in dark mode

## Quick Win 2: Standardize Simple Input Dialogs
**Files**: 3 files, ~20 minutes each
1. `qCC/ui_templates/askOneIntValueDlg.ui`
2. `qCC/ui_templates/askTwoDoubleValuesDlg.ui`
3. `qCC/ui_templates/askThreeDoubleValuesDlg.ui`

**Changes**:
- Remove hardcoded geometry from askThreeDoubleValuesDlg
- Standardize margins to 9px
- Add tab stops
- Add tooltips

**Impact**: More consistent and accessible simple dialogs

## Quick Win 3: Main Window Tooltips
**Files**: 1 file, ~1 hour
1. `qCC/ui_templates/mainWindow.ui`

**Changes**:
- Add tooltips to all toolbar buttons
- Add status tips to menu items

**Impact**: Immediate improvement in discoverability

---

# Appendix: Tools and Resources

## Recommended Tools

1. **Qt Designer** - Visual UI editing (included with Qt)
2. **Qt Creator** - IDE with excellent UI preview (optional)
3. **Python scripts** - For batch processing .ui files (XML)
4. **xmllint** - For validating .ui file XML structure
5. **Accessibility Insights** - For testing accessibility (Microsoft tool)
6. **Screen readers** - NVDA (Windows), VoiceOver (macOS), Orca (Linux)

## Useful Commands

```bash
# Find all .ui files with hardcoded geometry
grep -r "<rect>" --include="*.ui" qCC/ui_templates/

# Find all .ui files with hardcoded colors
grep -r "styleSheet" --include="*.ui" qCC/ libs/ plugins/

# Find dialogs without QDialogButtonBox
grep -L "QDialogButtonBox" qCC/ui_templates/*.ui

# Find dialogs without tab stops
grep -L "<tabstops>" qCC/ui_templates/*.ui

# Count tooltips in a file
grep -c "<string>.*</string>" qCC/ui_templates/somefile.ui
```

## Learning Resources

- Qt Documentation: https://doc.qt.io/qt-6/
- Qt Accessibility: https://doc.qt.io/qt-6/accessible.html
- Qt Style Sheets: https://doc.qt.io/qt-6/stylesheet.html
- Material Design Principles: https://m3.material.io/
- Apple Human Interface Guidelines: https://developer.apple.com/design/human-interface-guidelines/

---

**Document Version**: 1.0
**Last Updated**: 2025-11-23
**Author**: Claude
**Status**: Ready for Review
