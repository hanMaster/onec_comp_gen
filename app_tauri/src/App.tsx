import { Box, Container, Flex, Tabs } from "@mantine/core";
import "./App.css";
import FunctionPanel from "./components/FunctionPanel";
import ThemeSwitcher from "./components/ThemeSwitcher";
import BuildPanel from "./components/BuildPanel";


export default function App() {
  return (
    <main>
      <Flex justify={"end"} mr={5} mt={5}>
        <ThemeSwitcher />
      </Flex>

      <Container>
        <Tabs defaultValue="function" >
          <Tabs.List>
            <Tabs.Tab value="function">Функции</Tabs.Tab>
            <Tabs.Tab value="procedure">Процедуры</Tabs.Tab>
            <Tabs.Tab value="build">Сборка</Tabs.Tab>
          </Tabs.List>

          <Box mt={7}>
            <Tabs.Panel value="function">
              <FunctionPanel isFunction/>
            </Tabs.Panel>

            <Tabs.Panel value="procedure">
              <FunctionPanel isFunction={false}/>
            </Tabs.Panel>

            <Tabs.Panel value="build">
              <BuildPanel/>
            </Tabs.Panel>
          </Box>
        </Tabs>
      </Container>
    </main>
  );
}
