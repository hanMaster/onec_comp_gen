import {useComputedColorScheme,useMantineColorScheme, ActionIcon} from "@mantine/core";
import { IconSunMoon} from "@tabler/icons-react";

export default function ThemeSwitcher() {

    const { setColorScheme } = useMantineColorScheme();
    const computedColorScheme = useComputedColorScheme("light", {
      getInitialValueInEffect: true,
    });
  
    return (
      <ActionIcon
        onClick={() =>
          setColorScheme(computedColorScheme === "light" ? "dark" : "light")
        }
        variant="subtle"
        size="sm"
        color="gray"
      >
        <IconSunMoon />
      </ActionIcon>
    );
  
}
